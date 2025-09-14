#include "BotTelegram.h"
#include "JsonReader.h"
#include "good_funcs.h"
#include "json.hpp"
#include "PostgresDB.h"

#include <chrono>
#include <fstream>
#include <regex>

BotTelegram::BotTelegram(std::string offset) 
    : flag(true), worker(&BotTelegram::check_message, this), offset(std::move(offset)) 

{
    std::string conn = get_conn();
    PostgresDB db;
    std::vector<std::vector<std::string>> res;
    try{
        db.connect(conn);
        res = db.fetch(std::string("SELECT id, cards FROM users;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        res = db.fetch(std::string("SELECT id, cards FROM users;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        std::cout << e.what() << '\n';
        res = db.fetch(std::string("SELECT id, cards FROM users;"), std::vector<std::string>{});
    }
    for(const auto& row : res){
        std::string link = row[1], id = row[0];
        TelegramUser user(id);
        std::regex elementRegex(R"(\"([^"]+)\")");
        std::smatch match;
        auto begin = link.cbegin();
        auto end = link.cend();
        while(std::regex_search(begin, end, match, elementRegex)){
            user.add_product(std::string(match[1]));
            begin = match.suffix().first;
        }
        add_user(std::move(user));
    }
}

BotTelegramException::BotTelegramException(std::string str) : msg(std::move(str)) {}

BotTelegram::~BotTelegram(){
    flag = false;
    if(worker.joinable())
        worker.join();
}

BotTelegram::BotTelegram(BotTelegram&& obj) noexcept
    : flag(obj.flag.load()), worker(std::move(obj.worker)), offset(std::move(obj.offset))
{
    obj.flag = false;
}

BotTelegram& BotTelegram::operator=(BotTelegram&& obj) noexcept{
    if(this == &obj)
        return *this;

    stop();
    offset = std::move(obj.offset);
    flag = obj.flag.load();
    worker = std::move(obj.worker);
    obj.flag = false;
    return *this;
}

void BotTelegram::stop(){
    if(flag)
        flag = false;

    if(worker.joinable())
        worker.join();
}

const char* BotTelegramException::what() const noexcept{
    return msg.c_str();
}

void BotTelegram::check_message(){
    while(flag.load()){
        auto ptr = TelegramSender::get_instance();
        ptr->call(std::string(""), type_msg::read, std::string(offset));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto v = JsonReader::read("jq -r '.result[] | {text: .message.text, id:.message.from.id, update_id: .update_id}' ../res/result_"+ offset +".json", type_json::message);
        if(!v.empty()){
            std::string id = v[2];
            auto message = get_command_and_data(v[1]);
            std::string command = message.first;
            std::string data = message.second;
            
            id = id.substr(0, id.length() - 1);

            if(command == "/start"){
                users.insert({id, TelegramUser(id)});
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, std::string("Привет, теперь тебе доступен ряд команд для манипуляции с карточками\n"));
                PostgresDB db;
                db.connect(get_conn());
                db.execute(std::string("INSERT INTO users VALUES($1)"), std::vector<std::string>{id});                
            }
            else if(command == "/add_card"){
                if(data.size() == 0){
                    auto ptr = TelegramSender::get_instance();
                    ptr->call(id, type_msg::send, std::string("Вы ввели пустые данные\n"));
                    continue;
                }
                auto user = users.find(id);
                user->second.add_product(std::string(data));
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, std::string("Карточка добавлена\n"));
                PostgresDB db;
                std::string conn = get_conn();
                try{
                    db.connect(conn);
                    db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                } catch(BadConnectionDBexception& e) {
                    db.connect(conn);
                    db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                } catch(ErrorQueryResultDBexception& e) {
                    std::cout << e.what() << '\n';
                    db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                }
            }
            else if(command == "/del_card"){
                if(data.size() == 0){
                    auto ptr = TelegramSender::get_instance();
                    ptr->call(id, type_msg::send, std::string("Вы ввели пустые данные\n"));
                    continue;
                }
                auto user = users.find(id);
                user->second.del_product(data);
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, std::string("Карточка удалена\n"));
                PostgresDB db;
                std::string conn = get_conn();
                try{
                    db.connect(conn);
                    db.execute(std::string("UPDATE users SET cards = array_remove(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                } catch(BadConnectionDBexception& e) {
                    db.connect(conn);
                    db.execute(std::string("UPDATE users SET cards = array_remove(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                } catch(ErrorQueryResultDBexception& e) {
                    std::cout << e.what() << '\n';
                    db.execute(std::string("UPDATE users SET cards = array_remove(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
                }
            }
            else if(command == "/status"){
                auto user = users.find(id);
                std::string result = "Ваши скидки:\n";
                nlohmann::json data;
                std::ifstream file("../res/products.json");
                data = nlohmann::json::parse(file);
                for(const auto& obj : data["products"]){
                    std::string card = obj["text"];
                    std::string price = obj["price"];
                    if(user->second.is_has_product(card))
                        result += (card + " " + price + '\n');
                }
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, result);
            }
            else if(command == "/my_cards"){
                auto user = users.find(id);
                std::string result = "Ваши карточки:\n";
                auto cards = user->second.get_cards();
                for(const auto& obj : cards){
                    result += obj;
                }
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, result);
            }
            else{
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, std::string("Неверная команда"));
            }

            offset_reload();
        }
    }
}

std::pair<std::string, std::string> BotTelegram::get_command_and_data(const std::string& message) noexcept{
    std::string command, data;
    std::size_t spacePos = message.find(' ');
    if(spacePos != std::string::npos){
        command = message.substr(0, spacePos);
        data = message.substr(spacePos+1);
    } else {
        command = message;
    }
    return {command, data};
}

void BotTelegram::offset_reload(){
    long long oset = std::stoll(offset);
    oset++;
    offset = std::to_string(oset);
    update_offset(offset);
}