#include "BotTelegram.h"
#include "JsonReader.h"
#include "good_funcs.h"
#include "json.hpp"
#include "PostgresDB.h"
#include "Forecast.h"

#include <chrono>
#include <fstream>
#include <regex>
#include <vector>
#include <iostream>

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
                command_start(std::move(id));
            }
            else if(command == "/add_card"){
                command_add_card(std::move(id), std::move(data));
            }
            else if(command == "/del_card"){
                command_del_card(std::move(id), std::move(data));
            }
            else if(command == "/status"){
                command_status(std::move(id));
            }
            else if(command == "/my_cards"){
                command_my_cards(std::move(id));
            }
            else if(command == "/forecast"){
                command_forecast(std::move(id), std::move(data));
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

void BotTelegram::command_start(std::string&& id){
    users.insert({id, TelegramUser(id)});
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, std::string("Привет, теперь тебе доступен ряд команд для манипуляции с карточками\n"));
    PostgresDB db;
    try{
        db.connect(get_conn());
    } catch(BadConnectionDBexception& e){
        db.connect(get_conn());
    }
    try{
        db.execute(std::string("INSERT INTO users VALUES($1)"), std::vector<std::string>{id});
    } catch (BadConnectionDBexception& e){
        db.connect(get_conn());
        db.execute(std::string("INSERT INTO users VALUES($1)"), std::vector<std::string>{id});
    } catch (ErrorQueryResultDBexception& e){
        std::cout << e.what() << '\n';
        db.execute(std::string("INSERT INTO users VALUES($1)"), std::vector<std::string>{id});
    }
}

void BotTelegram::command_add_card(std::string&& id, std::string&& data){
    if(data.empty()){
        auto ptr = TelegramSender::get_instance();
        ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        offset_reload();
        return;
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
        db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
    }
}

void BotTelegram::command_del_card(std::string&& id, std::string&& data){
    if(data.empty()){
        auto ptr = TelegramSender::get_instance();
        ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        offset_reload();
        return;
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
        db.execute(std::string("UPDATE users SET cards = array_remove(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
    }
}

void BotTelegram::command_status(std::string&& id){
auto user = users.find(id);
    std::string result = "Ваши скидки:\n";
    nlohmann::json data;
    std::ifstream file("../res/products_discount.json");
    data = nlohmann::json::parse(file);
    for(const auto& obj : data["products"]){
        std::string card = obj["title"];
        if(!user->second.is_has_product(card))
            return;
        std::string price = obj["price"];
        if(obj.contains("discount")){
            std::string discount = obj["discount"];
            if(user->second.is_has_product(card))
                result += (card + "\nцена: " + price + "\nскидка: " + discount + '\n');
        }
        else{
            result += (card + " цена: " + price + '\n');
        }
    }
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, result);
}

void BotTelegram::command_my_cards(std::string&& id){
    auto user = users.find(id);
    std::string result = "Ваши карточки:\n";
    auto cards = user->second.get_cards();
    for(const auto& obj : cards){
        result += obj + "\n";
    }
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, result);
}

void BotTelegram::command_forecast(std::string&& id, std::string&& data){
    if(data.empty()){
        auto ptr = TelegramSender::get_instance();
        ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        offset_reload();
        return;
    }
    PostgresDB db;
    db.connect(get_conn());
    std::vector<std::vector<std::string>> query_result;
    try{
        query_result = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
    } catch(BadConnectionDBexception& e){
        db.connect(get_conn());
        query_result = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
    } catch(ErrorQueryResultDBexception& e){
        std::cout << e.what() << '\n';
        query_result = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
    }
    if(query_result.empty()){
        auto ptr = TelegramSender::get_instance();
        ptr->call(id, type_msg::send, std::string("Данной карточки нет в базе данных\n"));
        offset_reload();
        return;
    }
    std::vector<int> frequency;
    std::vector<std::chrono::sys_days> dates;
    for(const auto& obj : query_result){
        auto date = converte_string(obj[0]);
        auto days = std::chrono::sys_days{date};
        dates.emplace_back(days);
    }
    for(int i = 1; i < dates.size(); ++i){
        auto diff = (dates[i]-dates[i-1]).count()/7;
        frequency.emplace_back(diff);
    }
    if(frequency.size() < 5){
        auto ptr = TelegramSender::get_instance();
        ptr->call(id, type_msg::send, std::string("Слишком мало данных для прогнозирования\n"));
        offset_reload();
        return;
    }
    Forecast f;
    double prob = f.geometric_probability(std::move(frequency), 1);
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, std::string("Вероятность скидки на данный товар: " + std::to_string(static_cast<int>(prob * 100)) + "%"));
}