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
    Matrix* m = new Matrix;
    this->add_observer(m);

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

BotTelegram::~BotTelegram()
{
    flag = false;
    if(worker.joinable())
        worker.join();
}

BotTelegram::BotTelegram(BotTelegram&& obj) noexcept
    : flag(obj.flag.load()), worker(std::move(obj.worker)), offset(std::move(obj.offset))
{
    obj.flag = false;
}

BotTelegram& BotTelegram::operator=(BotTelegram&& obj) noexcept
{
    if(this == &obj)
        return *this;

    stop();
    offset = std::move(obj.offset);
    flag = obj.flag.load();
    worker = std::move(obj.worker);
    obj.flag = false;
    return *this;
}

void BotTelegram::stop()
{
    if(flag)
        flag = false;

    if(worker.joinable())
        worker.join();
}

const char* BotTelegramException::what() const noexcept
{
    return msg.c_str();
}

void BotTelegram::check_message()
{
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
            else if(command == "/recommendations"){
                command_recommendations(std::move(id));
            }
            else{
                auto ptr = TelegramSender::get_instance();
                ptr->call(id, type_msg::send, std::string("Неверная команда"));
            }

            offset_reload();
        }
    }
}

std::pair<std::string, std::string> BotTelegram::get_command_and_data(const std::string& message) noexcept
{
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

void BotTelegram::offset_reload()
{
    long long oset = std::stoll(offset);
    oset++;
    offset = std::to_string(oset);
    update_offset(offset);
}

void BotTelegram::command_start(std::string&& id)
{
    TelegramUser user(id);
    this->add_user(user);
    
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, std::string("Привет, теперь тебе доступен ряд команд для манипуляции с карточками\n"));
    PostgresDB db;
    try{
        db.connect(get_conn());
    } catch(BadConnectionDBexception& e){
        db.connect(get_conn());
    }
    try{
        db.execute(std::string("INSERT INTO users (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;"), std::vector<std::string>{id});
    } catch (BadConnectionDBexception& e){
        db.connect(get_conn());
        db.execute(std::string("INSERT INTO users (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;"), std::vector<std::string>{id});
    } catch (ErrorQueryResultDBexception& e){
        std::cout << e.what() << '\n';
        db.execute(std::string("INSERT INTO users (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;"), std::vector<std::string>{id});
    }
    
}

void BotTelegram::command_add_card(std::string&& id, std::string&& data)
{
    if(data.empty()){
        auto ptr = TelegramSender::get_instance();
        ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        offset_reload();
        return;
    }
    auto user = users.find(id);
    static std::unordered_map<std::string, std::vector<std::string>> hash_map {
        {"яблоко", {"Яблоки Global Village Роял Гала", "Яблоки Global Village Голден фасованные", "Яблоки красные", "Яблоки Голден", "Яблоки Гренни Смит", "Яблоки фасованные", "Яблоко Global Village красное", "Яблоко сезонное", "Яблоки Ред Делишес"}},
        {"яблоки", {"Яблоки Global Village Роял Гала", "Яблоки Global Village Голден фасованные", "Яблоки красные", "Яблоки Голден", "Яблоки Гренни Смит", "Яблоки фасованные", "Яблоко Global Village красное", "Яблоко сезонное", "Яблоки Ред Делишес"}},
        {"Яблоко", {"Яблоки Global Village Роял Гала", "Яблоки Global Village Голден фасованные", "Яблоки красные", "Яблоки Голден", "Яблоки Гренни Смит", "Яблоки фасованные", "Яблоко Global Village красное", "Яблоко сезонное", "Яблоки Ред Делишес"}},
        {"Яблоки", {"Яблоки Global Village Роял Гала", "Яблоки Global Village Голден фасованные", "Яблоки красные", "Яблоки Голден", "Яблоки Гренни Смит", "Яблоки фасованные", "Яблоко Global Village красное", "Яблоко сезонное", "Яблоки Ред Делишес"}},
        {"Молоко 3.2%", {"Молоко Правильное Молоко пастеризованное 3.2-4% БЗМЖ 2л", "Молоко стерилизованное Агуша детское 3.2% с 3 лет 500г", "Молоко Станция Молочная пастеризованное 3.2% БЗМЖ 1.55л", "Молоко Село Зеленое пастеризованное 3.2% БЗМЖ 1.947л", "Молоко Простоквашино пастеризованное 3.2% БЗМЖ 1.4л", "Молоко М ультрапастеризованное 3.2% БЗМЖ 950г", "Молоко С Нашей Фермы ультрапастеризованное 3.2% БЗМЖ 925мл", "Молоко Лебедяньмолоко 3.2% БЗМЖ 1400г", "Молоко Станция молочная топленое 3.2% БЗМЖ 900мл", "Молоко Вкуснотеево пастеризованное 3.2% БЗМЖ 900г", "Молоко ЭкоНива ультрапастеризованное 3.2% БЗМЖ 1л", "Молоко Лебедяньмолоко 3.2% БЗМЖ 900г", "Молоко Станция Молочная ультрапастеризованное 3.2% БЗМЖ 970мл", "Молоко Домик в деревне пастеризованное 3.2% БЗМЖ 1.44кг", "Молоко ЭкоНива детское ультрапастеризованное 3.2% БЗМЖ 200мл", "Молоко Сарафаново детское ультрапастеризованное 3.2% БЗМЖ 970мл", "Молоко Добрино питьевое пастеризованное 3.2% БЗМЖ 900г", "Молоко ЭкоНива пастеризованное цельное 3.3–6% БЗМЖ 1л"}},
        {"молоко 3.2%", {"Молоко Правильное Молоко пастеризованное 3.2-4% БЗМЖ 2л", "Молоко стерилизованное Агуша детское 3.2% с 3 лет 500г", "Молоко Станция Молочная пастеризованное 3.2% БЗМЖ 1.55л", "Молоко Село Зеленое пастеризованное 3.2% БЗМЖ 1.947л", "Молоко Простоквашино пастеризованное 3.2% БЗМЖ 1.4л", "Молоко М ультрапастеризованное 3.2% БЗМЖ 950г", "Молоко С Нашей Фермы ультрапастеризованное 3.2% БЗМЖ 925мл", "Молоко Лебедяньмолоко 3.2% БЗМЖ 1400г", "Молоко Станция молочная топленое 3.2% БЗМЖ 900мл", "Молоко Вкуснотеево пастеризованное 3.2% БЗМЖ 900г", "Молоко ЭкоНива ультрапастеризованное 3.2% БЗМЖ 1л", "Молоко Лебедяньмолоко 3.2% БЗМЖ 900г", "Молоко Станция Молочная ультрапастеризованное 3.2% БЗМЖ 970мл", "Молоко Домик в деревне пастеризованное 3.2% БЗМЖ 1.44кг", "Молоко ЭкоНива детское ультрапастеризованное 3.2% БЗМЖ 200мл", "Молоко Сарафаново детское ультрапастеризованное 3.2% БЗМЖ 970мл", "Молоко Добрино питьевое пастеризованное 3.2% БЗМЖ 900г", "Молоко ЭкоНива пастеризованное цельное 3.3–6% БЗМЖ 1л"}},
        {"Молоко 2.5%", {"Молоко Станция Молочная пастеризованное 2.5% БЗМЖ 900мл", "Молоко Домик в деревне пастеризованное 2.5% БЗМЖ 930мл", "Молоко Искренне Ваш пастеризованное 2.5% БЗМЖ 930г", "Молоко Станция молочная ультрапастеризованное 2.5% БЗМЖ 970мл", "Молоко козье Село Зеленое цельное ультрапастеризованное 2.8-5.6% БЗМЖ 200г", "Молоко Экомилк пастеризованное 2.5% БЗМЖ 900мл", "Молоко Вкус & Польза пастеризованное 2.5% БЗМЖ 900г", "Молоко Сарафаново детское ультрапастеризованное 2.5% БЗМЖ 970мл", "Молоко Сарафаново пастеризованное 2.5% БЗМЖ 930мл"}},
        {"молоко 2.5%", {"Молоко Станция Молочная пастеризованное 2.5% БЗМЖ 900мл", "Молоко Домик в деревне пастеризованное 2.5% БЗМЖ 930мл", "Молоко Искренне Ваш пастеризованное 2.5% БЗМЖ 930г", "Молоко Станция молочная ультрапастеризованное 2.5% БЗМЖ 970мл", "Молоко козье Село Зеленое цельное ультрапастеризованное 2.8-5.6% БЗМЖ 200г", "Молоко Экомилк пастеризованное 2.5% БЗМЖ 900мл", "Молоко Вкус & Польза пастеризованное 2.5% БЗМЖ 900г", "Молоко Сарафаново детское ультрапастеризованное 2.5% БЗМЖ 970мл", "Молоко Сарафаново пастеризованное 2.5% БЗМЖ 930мл"}},
        {"Чай", {"Чай черный Assand Famous Ceylon 100х2г", "Чай Tess Санрайз черный в пакетиках 100х1.8г", "Чай черный Принцесса Нури Высокогорный 100х2г", "Чай Greenfield Kenyan Sunrise черный в пакетиках 100х2г", "Чай Greenfield Earl Grey Fantasy черный с бергамотом 100х2г", "Чай черный Richard Королевский английский завтрак 100x2г", "Чай черный Richard Royal ceylon байховый 100х2г", "Чай черный Richard Royal Peach & Mint с ароматом персика и мяты 25х1.7г", "Чай черный Akbar Akbar Mountain Fresh 25х2г", "Чай черный Tess Санрайз в пакетиках 25х1.8г", "Чай черный Майский Лесные ягоды 25х1.5г", "Чай Greenfield Kenyan Sunrise черный в пакетиках 25х2г", "Чай черный Richard Royal Berries Selection лесные ягоды 25х1.7г", "Чай черный Майский Отборный Цейлон 25х2г", "Чай черный Принцесса Нури Высокогорный 25х2г", "Чай черный Akbar GFold цейлонский 25х2г"}}, 
        {"чай", {"Чай черный Assand Famous Ceylon 100х2г", "Чай Tess Санрайз черный в пакетиках 100х1.8г", "Чай черный Принцесса Нури Высокогорный 100х2г", "Чай Greenfield Kenyan Sunrise черный в пакетиках 100х2г", "Чай Greenfield Earl Grey Fantasy черный с бергамотом 100х2г", "Чай черный Richard Королевский английский завтрак 100x2г", "Чай черный Richard Royal ceylon байховый 100х2г", "Чай черный Richard Royal Peach & Mint с ароматом персика и мяты 25х1.7г", "Чай черный Akbar Akbar Mountain Fresh 25х2г", "Чай черный Tess Санрайз в пакетиках 25х1.8г", "Чай черный Майский Лесные ягоды 25х1.5г", "Чай Greenfield Kenyan Sunrise черный в пакетиках 25х2г", "Чай черный Richard Royal Berries Selection лесные ягоды 25х1.7г", "Чай черный Майский Отборный Цейлон 25х2г", "Чай черный Принцесса Нури Высокогорный 25х2г", "Чай черный Akbar GFold цейлонский 25х2г"}}, 
        {"бананы", {"Бананы Global Village", "Бананы Красная Цена фасованные"}},
        {"Бананы", {"Бананы Global Village", "Бананы Красная Цена фасованные"}},
        {"сыр", {"Сыр Брест-Литовск сливочный 50% БЗМЖ 200г", "Сыр Брест-Литовск маасдам 45% БЗМЖ 200г", "Сыр Liebendorf Тильзитер 45% БЗМЖ", "Сыр Liebendorf Гауда полутвердый 45% БЗМЖ", "Сыр Сыробогатов Швейцарский 45% БЗМЖ 180г"}},
        {"Сыр", {"Сыр Брест-Литовск сливочный 50% БЗМЖ 200г", "Сыр Брест-Литовск маасдам 45% БЗМЖ 200г", "Сыр Liebendorf Тильзитер 45% БЗМЖ", "Сыр Liebendorf Гауда полутвердый 45% БЗМЖ", "Сыр Сыробогатов Швейцарский 45% БЗМЖ 180г"}},
        {"Йогурты", {"Йогурт Teos греческий 2% 250г", "Йогурт Teos Греческий Черника 2% 140г", "Йогурт Teos Греческий 2% 140г", "Йогурт Teos греческий клубника 2% 140г", "Йогурт питьевой Чудо клубника-земляника 1.9% 260мл", "Йогурт питьевой Чудо ягодное мороженое 1.9% 260мл", "Йогурт питьевой Чудо персик-манго-дыня 1.9% 260мл"}},
        {"йогурты", {"Йогурт Teos греческий 2% 250г", "Йогурт Teos Греческий Черника 2% 140г", "Йогурт Teos Греческий 2% 140г", "Йогурт Teos греческий клубника 2% 140г", "Йогурт питьевой Чудо клубника-земляника 1.9% 260мл", "Йогурт питьевой Чудо ягодное мороженое 1.9% 260мл", "Йогурт питьевой Чудо персик-манго-дыня 1.9% 260мл"}},
        {"Йогурт", {"Йогурт Teos греческий 2% 250г", "Йогурт Teos Греческий Черника 2% 140г", "Йогурт Teos Греческий 2% 140г", "Йогурт Teos греческий клубника 2% 140г", "Йогурт питьевой Чудо клубника-земляника 1.9% 260мл", "Йогурт питьевой Чудо ягодное мороженое 1.9% 260мл", "Йогурт питьевой Чудо персик-манго-дыня 1.9% 260мл"}},
        {"йогурт", {"Йогурт Teos греческий 2% 250г", "Йогурт Teos Греческий Черника 2% 140г", "Йогурт Teos Греческий 2% 140г", "Йогурт Teos греческий клубника 2% 140г", "Йогурт питьевой Чудо клубника-земляника 1.9% 260мл", "Йогурт питьевой Чудо ягодное мороженое 1.9% 260мл", "Йогурт питьевой Чудо персик-манго-дыня 1.9% 260мл"}},
        {"Яйцо 1с", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"Яйца 1с", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"яйцо 1с", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"яйца 1с", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"Яйца 1С", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"Яйцо 1С", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"яйцо 1С", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"яйца 1С", {"Яйца куриные Выручай С1 10шт.", "Яйца Ешь хорошее куриные столовые С1 10шт.", "Яйца куриные Выручай С1 20шт.", "Яйцо куриное Лето пищевое С1 10шт.", "Яйцо куриное Окское столовое С1 10шт."}},
        {"Яйцо 0с", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"Яйца 0с", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"яйца 0с", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"яйцо 0с", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"Яйцо 0С", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"Яйца 0С", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"яйцо 0С", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"яйца 0С", {"Яйца Ешь Хорошее куриные столовые СО 10шт.", "Яйца куриные Выручай СО 10шт.", "Яйцо куриное Окское столовое СО 10шт.", "Яйцо куриное Село Зеленое Деревенское С0 10шт.", "Яйцо куриное Лето столовое СО 10шт."}},
        {"Стиральный порошок", {"Стиральный порошок Tide Color автомат 1.5кг", "Порошок стиральный Лоск Color Total 2.7кг", "Стиральный порошок Tide Color автомат 450г", "Порошок стиральный Персил Color Свежесть от Vernel 450г", "Порошок Персил Свежесть стиральный 3кг", "Стиральный порошок Tide Color автомат 3кг", "Стиральный порошок Ariel Color автомат 1.5кг", "Порошок BiMax 100 пятен стиральный автомат 3кг", "Порошок стиральный BiMax Белоснежные вершины 3кг", "Порошок стиральный Лоск Active 6 Горное Озеро автомат 2.7кг", "Порошок стиральный Персил Color Свежесть от Вернель 3кг", "Стиральный порошок Ariel Color с ароматом от Lenor автомат 3кг", "Стиральный порошок Ariel Горный родник автомат 3кг", "Стиральный порошок Ariel Color автомат 3кг", "Порошок стиральный Персил Color свежесть от Vernel 2.1кг"}},
        {"стиральный порошок", {"Стиральный порошок Tide Color автомат 1.5кг", "Порошок стиральный Лоск Color Total 2.7кг", "Стиральный порошок Tide Color автомат 450г", "Порошок стиральный Персил Color Свежесть от Vernel 450г", "Порошок Персил Свежесть стиральный 3кг", "Стиральный порошок Tide Color автомат 3кг", "Стиральный порошок Ariel Color автомат 1.5кг", "Порошок BiMax 100 пятен стиральный автомат 3кг", "Порошок стиральный BiMax Белоснежные вершины 3кг", "Порошок стиральный Лоск Active 6 Горное Озеро автомат 2.7кг", "Порошок стиральный Персил Color Свежесть от Вернель 3кг", "Стиральный порошок Ariel Color с ароматом от Lenor автомат 3кг", "Стиральный порошок Ariel Горный родник автомат 3кг", "Стиральный порошок Ariel Color автомат 3кг", "Порошок стиральный Персил Color свежесть от Vernel 2.1кг"}},
        {"кукуруза", {"Кукуруза консервированная Бондюэль сладкая 170г", "Кукуруза Global Village Selection сладкая 340г", "Кукуруза Global Village Selection без сахара 340г", "Кукуруза Global Village сахарная 340г", "Кукуруза консервированная Бондюэль молодая 170г", "Кукуруза консервированная Бондюэль молодая 340г", "Кукуруза консервированная Бондюэль сладкая 340г"}},
        {"Кукуруза", {"Кукуруза консервированная Бондюэль сладкая 170г", "Кукуруза Global Village Selection сладкая 340г", "Кукуруза Global Village Selection без сахара 340г", "Кукуруза Global Village сахарная 340г", "Кукуруза консервированная Бондюэль молодая 170г", "Кукуруза консервированная Бондюэль молодая 340г", "Кукуруза консервированная Бондюэль сладкая 340г"}},
        {"горох", {"Горошек Global Village Selection зеленый 400г", "Горошек Global Village Selection Молодой без добавления сахара из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 200г", "Горошек Global Village зеленый из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 400г"}},
        {"Горох", {"Горошек Global Village Selection зеленый 400г", "Горошек Global Village Selection Молодой без добавления сахара из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 200г", "Горошек Global Village зеленый из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 400г"}},
        {"горошек", {"Горошек Global Village Selection зеленый 400г", "Горошек Global Village Selection Молодой без добавления сахара из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 200г", "Горошек Global Village зеленый из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 400г"}},
        {"Горошек", {"Горошек Global Village Selection зеленый 400г", "Горошек Global Village Selection Молодой без добавления сахара из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 200г", "Горошек Global Village зеленый из мозговых сортов 400г", "Горошек консервированный Бондюэль нежный 400г"}}
        // {"огурцы солёные"},
        // {"тушёнка"}
    };

    PostgresDB db;
    std::string conn = get_conn();
    try{
        db.connect(conn);
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
    }

    auto it = hash_map.find(std::string(data));
    bool found = false;
    if(it != hash_map.end()){
        found = true;
        for(const auto& obj : it->second){
            user->second.add_product(obj);
            try{
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{obj, id});
            } catch(BadConnectionDBexception& e){
                db.connect(conn);
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{obj, id});
            } catch(ErrorQueryResultDBexception& e){
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{obj, id});
            }
        }
    }
    else{
        std::vector<std::vector<std::string>> result_query_found;
        try{
            result_query_found = db.fetch(std::string("SELECT EXISTS (SELECT 1 FROM cards WHERE title = $1);"), std::vector<std::string>{data});
        } catch (BadConnectionDBexception& e){
            db.connect(conn);
            result_query_found = db.fetch(std::string("SELECT EXISTS (SELECT 1 FROM cards WHERE title = $1);"), std::vector<std::string>{data});
        } catch (ErrorQueryResultDBexception& e){
            result_query_found = db.fetch(std::string("SELECT EXISTS (SELECT 1 FROM cards WHERE title = $1);"), std::vector<std::string>{data});
        }
        if(result_query_found.empty() == 0){
            //critical error
            result_query_found.push_back(std::vector<std::string>{std::string{"0"}});
        }
        if(result_query_found[0][0] == "f"){
            found = false;
        } else {
            user->second.add_product(std::string(data));
            try{
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
            } catch(BadConnectionDBexception& e){
                db.connect(conn);
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
            } catch(ErrorQueryResultDBexception& e) {
                db.execute(std::string("UPDATE users SET cards = array_append(cards, $1) WHERE id = $2;"), std::vector<std::string>{data, id});
            }
        }
    }
    auto ptr = TelegramSender::get_instance();
    if(found)
        ptr->call(id, type_msg::send, std::string("Карточка добавлена\n"));
    else
        ptr->call(id, type_msg::send, std::string("Не удалось найти такую карточку в базу данных. Проверьте корректность названия карточки или же обратитесь к администратору\n"));
}

void BotTelegram::command_del_card(std::string&& id, std::string&& data)
{
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

void BotTelegram::command_status(std::string&& id)
{
    auto user = users.find(id);
    std::string result = "Ваши скидки:\n";
    nlohmann::json data;
    std::ifstream file("../sensetive_res/products_discount.json");
    data = nlohmann::json::parse(file);
    for(const auto& obj : data["products"]){
        std::string card = obj["title"];
        if(!user->second.is_has_product(card))
            continue;;
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

void BotTelegram::command_my_cards(std::string&& id)
{
    auto user = users.find(id);
    std::string result = "Ваши карточки:\n";
    auto cards = user->second.get_cards();
    for(const auto& obj : cards){
        result += obj + "\n";
    }
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, result);
}

void BotTelegram::command_forecast(std::string&& id, std::string&& data)
{
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
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
    } catch(BadConnectionDBexception& e){
        db.connect(get_conn());
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
    } catch(ErrorQueryResultDBexception& e){
        std::cout << e.what() << '\n';
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
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
    // if(frequency.size() < 5){
    //     auto ptr = TelegramSender::get_instance();
    //     ptr->call(id, type_msg::send, std::string("Слишком мало данных для прогнозирования\n"));
    //     offset_reload();
    //     return;
    // }
    Forecast f;
    double prob = f.geometric_probability(std::move(frequency), 0);
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, std::string("Вероятность скидки на данный товар: " + std::to_string(static_cast<int>(prob * 100)) + "%"));
}

void BotTelegram::command_recommendations(std::string&& id)
{
    auto res = observers[0]->recommendation(id);
    std::string result = "Рекомендуемые карточки\n";
    int count = 0;
    for(const auto& v : res){
        if(count == 3)
            break;
        result += v + '\n';
        count++;
    }
    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, result);
}

void BotTelegram::add_observer(IUserObserver* obs)
{
    observers.push_back(obs);
}

void BotTelegram::remove_observer(IUserObserver* obs)
{
    observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
}