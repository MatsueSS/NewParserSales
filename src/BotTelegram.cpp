#include "BotTelegram.h"

#include "good_funcs.h"
#include "json.hpp"
#include "PostgresDB.h"
#include "FactoryRecommendations.h"
#include "FactorySearcher.h"
#include "ModelSelector.h"
#include "PostgresDB.h"
#include "HiSquare.h"

#include <queue>
#include <iostream>

BotTelegram::BotTelegram(std::string offset, std::shared_ptr<PoolCards> ptr_pc, RecType rectype, ProdType prodtype) 
    : flag(true)
    , offset(std::move(offset))
    , ptr_pc(ptr_pc)
    , users(std::make_shared<std::unordered_map<std::string, TelegramUser>>())
    , observer(FactoryRecommendations::create(rectype, ptr_pc, users))
    , searcher(FactoryMatcher::create(prodtype, "../sensetive_res/new_dict.txt", ptr_pc))
    , fm(std::initializer_list<TypeModel>{TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL})
    , ptr_mx(std::make_unique<std::mutex>())
{
    load_users_from_db();
    init_tree();

    worker = std::thread(&BotTelegram::check_message, this);
}

void BotTelegram::load_users_from_db()
{
    std::string conn = get_conn();
    PostgresDB db;
    db.connect(conn);
    std::vector<std::vector<std::string>> res;

    try{
        res = db.fetch(std::string("SELECT id FROM users;"), std::vector<std::string>{});
    } catch (BadConnectionDBexception& e){
        db.connect(conn);
        res = db.fetch(std::string("SELECT id FROM users;"), std::vector<std::string>{});
    } catch (ErrorQueryResultDBexception& e){
        res = db.fetch(std::string("SELECT id FROM users;"), std::vector<std::string>{});
    }

    for(const auto& cont : res){
        TelegramUser user(cont[0], ptr_pc);
        add_user(std::move(user));
    }

    res.clear();

    try{
        res = db.fetch(std::string("SELECT id, preference FROM preferences;"), std::vector<std::string>{});
    } catch (BadConnectionDBexception& e){
        db.connect(conn);
        res = db.fetch(std::string("SELECT id, preference FROM preferences;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        res = db.fetch(std::string("SELECT id, preference FROM preferences;"), std::vector<std::string>{});
    }

    for(const auto& cont : res){
        auto it = users->find(cont[0]);
        if(it == users->end()){
            throw BotTelegramException("critical error with starting programm - the user is missing from the users table, but is present in the preferences table");
        }
        it->second.add_product(cont[1]);
    }
}

BotTelegramException::BotTelegramException(std::string str) : msg(std::move(str)) {}
BotTelegramException::BotTelegramException(const BotTelegramException& obj) : msg(obj.msg) {}

BadInitBotTelegramException::BadInitBotTelegramException(std::string msg) : BotTelegramException(std::move(msg)) {}

BotTelegram::~BotTelegram()
{
    flag = false;
    if(worker.joinable())
        worker.join();
}

BotTelegram::BotTelegram(BotTelegram&& obj) noexcept
    : users(std::move(obj.users)), flag(obj.flag.load()), worker(std::move(obj.worker)), offset(std::move(obj.offset))
    , users_with_keyboard(std::move(obj.users_with_keyboard)), ptr_pc(std::move(obj.ptr_pc)), observer(std::move(obj.observer))
    , searcher(std::move(obj.searcher)), tree(std::move(obj.tree)), MachineState(std::move(obj.MachineState)), fm(std::move(obj.fm))
    , ts(std::move(obj.ts)), ptr_mx(std::move(obj.ptr_mx))
{
    obj.flag = false;
}

BotTelegram& BotTelegram::operator=(BotTelegram&& obj) noexcept
{
    if(this == &obj)
        return *this;

    stop();
    users = std::move(users);
    flag = obj.flag.load();
    worker = std::move(obj.worker);
    offset = std::move(obj.offset);
    users_with_keyboard = std::move(obj.users_with_keyboard);
    observer = std::move(obj.observer);
    searcher = std::move(obj.searcher);
    tree = std::move(obj.tree);
    MachineState = std::move(obj.MachineState);
    fm = std::move(obj.fm);
    ts = std::move(obj.ts);
    ptr_mx = std::move(obj.ptr_mx);

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
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> locker(curl_mutex);
        ts.read(offset);
        auto data = ts.get_response();
        locker.unlock();

        if(data.empty()){
            std::cerr << "ERROR: Empty response from Telegram\n";

            ts.close();
            ts.build();
            continue;
        }

        if(data == "{\"ok\":true,\"result\":[]}") continue;

        std::string id, full_message;
        try{
            nlohmann::json js = nlohmann::json::parse(data);
            id = js["result"][0]["message"]["from"]["id"].dump();
            full_message = js["result"][0]["message"]["text"];
        } catch(nlohmann::json::parse_error& e){
            continue;
        }

        if(users_with_keyboard.find(id) == users_with_keyboard.end()){
            send_main_keyboard(id);
            users_with_keyboard.insert(id);
        }

        auto waiting = MachineState.get_waiting(id);
        if(waiting != UserStateMachine::UserAction::NONE){
            if(waiting == UserStateMachine::UserAction::ADD_CARD)
                command_add_card(std::string(id), std::string(full_message));
            else if(waiting == UserStateMachine::UserAction::DEL_CARD)
                command_del_card(std::string(id), std::string(full_message));
            else if(waiting == UserStateMachine::UserAction::FORECAST)
                command_forecast(std::string(id), std::string(full_message));
            else if(waiting == UserStateMachine::UserAction::HAS_DISCOUNT)
                command_has_discount(std::string(id), std::string(full_message));
            MachineState.clear(id);
            offset_reload();
            continue;
        }

        if(full_message == "/start") {
            command_start(std::move(id));
        }
        else if (full_message == "📋 Мои карточки") {
            command_my_cards(std::move(id));
        }
        else if (full_message == "💰 Статус скидок") {
            command_status(std::move(id));
        }
        else if (full_message == "🎯 Рекомендации") {
            command_recommendations(std::move(id));
        }
        else if (full_message == "➕ Добавить карточку") {
            MachineState.set_waiting(id, UserStateMachine::UserAction::ADD_CARD);
            safety_writter(id, std::string("Введите название товара для добавления:"), locker);
        }
        else if (full_message == "➖ Удалить карточку") {
            MachineState.set_waiting(id, UserStateMachine::UserAction::DEL_CARD);
            safety_writter(id, std::string("Введите название товара для удаления:"), locker);
        }
        else if (full_message == "📊 Прогноз") {
            MachineState.set_waiting(id, UserStateMachine::UserAction::FORECAST);
            safety_writter(id, std::string("Введите название товара для прогноза:"), locker);
        }
        else if(full_message == "❓ Узнать скидку"){
            MachineState.set_waiting(id, UserStateMachine::UserAction::HAS_DISCOUNT);
            safety_writter(id, std::string("Введите название товара для проверки скидки:"), locker);
        }
        else {
            safety_writter(id, std::string("Используйте кнопки меню"), locker);
        }

        offset_reload();
    }
}

void BotTelegram::send_main_keyboard(const std::string& id) noexcept
{
    std::string keyboard_json = R"({
        "keyboard": [
            [{"text": "📋 Мои карточки"}, {"text": "💰 Статус скидок"}],
            [{"text": "🎯 Рекомендации"}],
            [{"text": "➕ Добавить карточку"}, {"text": "➖ Удалить карточку"}],
            [{"text": "📊 Прогноз"}, {"text": "❓ Узнать скидку"}]
        ],
        "resize_keyboard": true,
        "one_time_keyboard": false
    })";
    
    std::string message = 
        "🌟 Главное меню\n\n"
        "🔹 Нажимайте кнопки для команд\n"
        "🔹 Для добавления/удаления/прогноза введите название после нажатия";
    
    std::unique_lock<std::mutex> lock(curl_mutex);
    ts.send_keyboard(id, message, keyboard_json);
}

void BotTelegram::offset_reload()
{
    std::unique_lock<std::mutex> lock(*ptr_mx);
    long long oset = std::stoll(offset);
    oset++;
    offset = std::to_string(oset);
    update_offset(offset);
}

void BotTelegram::command_start(std::string&& id)
{
    TelegramUser user(id, ptr_pc);
    this->add_user(std::move(user));

    send_main_keyboard(id);
    
    std::unique_lock<std::mutex> lock(curl_mutex);
    ts.write(id, "Привет, теперь тебе доступен ряд команд для манипуляции с карточками");
    lock.unlock();
    PostgresDB db;
    db.connect(get_conn());
    try{
        db.execute(std::string("INSERT INTO users (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;"), std::vector<std::string>{id});
    } catch (BadConnectionDBexception& e){
        db.connect(get_conn());
        db.execute(std::string("INSERT INTO users (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;"), std::vector<std::string>{id});
    } catch (ErrorQueryResultDBexception& e){
        db.execute(std::string("INSERT INTO users (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;"), std::vector<std::string>{id});
    }
}

void BotTelegram::init_tree() noexcept
{
    PostgresDB db;
    db.connect(get_conn());
    std::vector<std::vector<std::string>> unique_card = db.fetch(std::string("SELECT title FROM products;"), std::vector<std::string>{});
    for(const auto& obj : unique_card){
        tree.add_word(obj[0]);
    }
}

void BotTelegram::command_has_discount(std::string&& id, std::string&& card)
{
    std::unique_lock<std::mutex> locker(curl_mutex, std::defer_lock);
    if(card.empty()){
        safety_writter(id, std::string("Вы не ввели данные"), locker);
        offset_reload();
        return;
    }
    auto user = users->find(id);

    PostgresDB db;
    std::string conn = get_conn();

    std::vector<std::vector<std::string>> row;
    std::string last_sat = saturday_to_string(get_previous_or_current_saturday());

    if(tree.has_prefix(card)){
        std::string true_card;
        true_card = tree.give_word_for_prefix(card);
        std::vector<std::vector<std::string>> res;
        try{
            db.connect(conn);
            res = db.fetch(std::string("SELECT EXISTS (SELECT DISTINCT ON(date) 1 FROM cards WHERE title = $1 AND date = $2 AND discount IS NOT NULL)"), std::vector<std::string>{true_card, last_sat});
        } catch (BadConnectionDBexception& e){
            db.connect(conn);
            res = db.fetch(std::string("SELECT EXISTS (SELECT DISTINCT ON(date) 1 FROM cards WHERE title = $1 AND date = $2 AND discount IS NOT NULL)"), std::vector<std::string>{true_card, last_sat});
        } catch (ErrorQueryResultDBexception& e){
            res = db.fetch(std::string("SELECT EXISTS (SELECT DISTINCT ON(date) 1 FROM cards WHERE title = $1 AND date = $2 AND discount IS NOT NULL)"), std::vector<std::string>{true_card, last_sat});
        }

        if(res[0][0] == "t"){
            safety_writter(id, std::string("Скидка на " + true_card + " присутствует (данные взяты на число - " + last_sat + ")"), locker);
        } else {
            safety_writter(id, std::string("Скидка на " + true_card + " отсутствует (данные взяты на число - " + last_sat + ")"), locker);
        }
    } else {
        safety_writter(id, std::string("Не удалось найти такую карточку в базе данных. Проверьте корректность названия карточки или же обратитесь к администратору"), locker);
    }
}

void BotTelegram::command_add_card(std::string&& id, std::string&& data)
{
    std::unique_lock<std::mutex> locker(curl_mutex, std::defer_lock);
    if(data.empty()){
        safety_writter(id, std::string("Вы не ввели данные"), locker);
        offset_reload();
        return;
    }
    auto user = users->find(id);
    
    PostgresDB db;
    std::string conn = get_conn();
    db.connect(get_conn());

    auto search_result = searcher.search(std::string(data));
    bool found = false;
    if(search_result){
        found = true;
        for(const auto& obj : *search_result){
            std::string temp = ptr_pc->get_title(obj).get_title();
            user->second.add_product(temp);
            try{
                db.execute(std::string("INSERT INTO preferences (id, preference) VALUES ($1, $2) ON CONFLICT (id, preference) DO NOTHING;"), std::vector<std::string>{id, temp});
            } catch(BadConnectionDBexception& e){
                db.connect(conn);
                db.execute(std::string("INSERT INTO preferences (id, preference) VALUES ($1, $2) ON CONFLICT (id, preference) DO NOTHING;"), std::vector<std::string>{id, temp});
            } catch(ErrorQueryResultDBexception& e){
                db.execute(std::string("INSERT INTO preferences (id, preference) VALUES ($1, $2) ON CONFLICT (id, preference) DO NOTHING;"), std::vector<std::string>{id, temp});
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

        if(result_query_found[0][0] == "f"){
            found = false;
        } else{
            found = true;
            user->second.add_product(std::string(data));
            try{
                db.execute(std::string("INSERT INTO preferences (id, preference) VALUES ($1, $2) ON CONFLICT (id, preference) DO NOTHING;"), std::vector<std::string>{id, data});
            } catch(BadConnectionDBexception& e){
                db.connect(conn);
                db.execute(std::string("INSERT INTO preferences (id, preference) VALUES ($1, $2) ON CONFLICT (id, preference) DO NOTHING;"), std::vector<std::string>{id, data});
            } catch(ErrorQueryResultDBexception& e) {
                db.execute(std::string("INSERT INTO preferences (id, preference) VALUES ($1, $2) ON CONFLICT (id, preference) DO NOTHING;"), std::vector<std::string>{id, data});
            }
        }
    }
    if(found){
        safety_writter(id, std::string("Карточка добавлена"), locker);
    } else{
        safety_writter(id, std::string("Не удалось найти такую карточку в базе данных. Проверьте корректность названия карточки или же обратитесь к администратору"), locker);
    }
}

void BotTelegram::command_del_card(std::string&& id, std::string&& data)
{
    std::unique_lock<std::mutex> locker(curl_mutex, std::defer_lock);
    if(data.empty()){
        safety_writter(id, std::string("Вы не ввели данные"), locker);
        offset_reload();
        return;
    }
    auto user = users->find(id);
    user->second.del_product(data);
    safety_writter(id, std::string("Карточка удалена"), locker);
    PostgresDB db;
    std::string conn = get_conn();
    try{
        db.connect(conn);
        db.execute(std::string("DELETE FROM preferences WHERE id = $1 AND preference = $2;"), std::vector<std::string>{id, data});
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
        db.execute(std::string("DELETE FROM preferences WHERE id = $1 AND preference = $2;"), std::vector<std::string>{id, data});
    } catch(ErrorQueryResultDBexception& e) {
        db.execute(std::string("DELETE FROM preferences WHERE id = $1 AND preference = $2;"), std::vector<std::string>{id, data});
    }
}

void BotTelegram::command_status(std::string&& id)
{
    // auto user = users->find(id);
    // std::string result = "Ваши скидки:\n";

    // PostgresDB db;
    // db.connect(get_conn());

    // auto res = db.fetch(std::string("SELECT title FROM cards WHERE date = $1 AND discount IS NOT NULL;"), std::vector<std::string>{converte_ymd(get_previous_or_current_saturday())});

    // for(const auto& obj : res){
    //     if(user->second.is_has_product(obj[0])) result += obj[0] + '\n';
    // }
    // //auto ptr = TelegramSender::get_instance();
    // //ptr->call(id, type_msg::send, result);
    // ts.write(id, result);

    PostgresDB db;
    db.connect(get_conn());

    auto res = db.fetch(std::string("SELECT t1.preference FROM (SELECT * FROM preferences WHERE id = $1) t1 INNER JOIN (SELECT title FROM cards WHERE date = $2 AND discount IS NOT NULL) t2 ON t1.preference = t2.title;"), std::vector<std::string>{id, converte_ymd(get_previous_or_current_saturday())});
    std::string result = "Ваши скидки:\n";
    for(const auto& obj : res){
        result += obj[0] + '\n';
    }
    std::unique_lock<std::mutex> locker(curl_mutex);
    ts.write(id, result);
}

void BotTelegram::command_my_cards(std::string&& id)
{
    auto user = users->find(id);
    std::string result = "Ваши карточки:\n";
    auto cards = user->second.get_cards();
    for(const auto& obj : cards){
        result += ptr_pc->get_title(obj).get_title() + "\n";
    }
    std::unique_lock<std::mutex> locker(curl_mutex);
    ts.write(id, result);
}

void BotTelegram::command_forecast(std::string&& id, std::string&& data)
{
    std::unique_lock<std::mutex> locker(curl_mutex, std::defer_lock);
    if(data.empty()){
        safety_writter(id, std::string("Вы не ввели данные"), locker);
        offset_reload();
        return;
    }

    int result;

    try{
        result = fm.get_probability(data);
    } catch(EmptySampleProbabilityModelException& e){
        safety_writter(id, std::string("Данной карточки нет в базе данных или же ещё не было скидок на этот товар"), locker);
        return;
    } catch(SmallSampleForecastManagerException& e){
        safety_writter(id, std::string("Слишком мало данных для такой карточки"), locker);
        return;
    } catch(NoSuitableProbabilityException& e){
        safety_writter(id, std::string("Данные не позволяют произвести прогнозирование"), locker);
        return;
    } catch (NoDataHiSquareException& e){
        safety_writter(id, std::string("Данные не позволяют произвести прогнозирование"), locker);
        return;
    }

    safety_writter(id, std::string("Вероятность скидки на данный товар: " + std::to_string(result) + "%"), locker);
}

void BotTelegram::command_recommendations(std::string&& id)
{
    auto res = observer.recommendation(id);
    std::string result = "Рекомендуемые карточки\n";
    int count = 0;
    for(const auto& v : res){
        if(count == 3)
            break;
        result += v + '\n';
        count++;
    }
    std::unique_lock<std::mutex> locker(curl_mutex);
    ts.write(id, result);
}

void BotTelegram::reset_cache() noexcept
{
    fm.reset_cache();
}