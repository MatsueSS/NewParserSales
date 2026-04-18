#include "BotTelegram.h"

#include "JsonReader.h"
#include "good_funcs.h"
#include "json.hpp"
#include "PostgresDB.h"
#include "FactoryRecommendations.h"
#include "FactorySearcher.h"
#include "ModelSelector.h"

#include <queue>

BotTelegram::BotTelegram(std::string offset, std::shared_ptr<PoolCards> ptr_pc, RecType rectype, ProdType prodtype) 
    : flag(true)
    , offset(std::move(offset))
    , ptr_pc(ptr_pc)
    , users(std::make_shared<std::unordered_map<std::string, TelegramUser>>())
    , observer(FactoryRecommendations::create(rectype, ptr_pc, users))
    , searcher(FactoryMatcher::create(prodtype, "../sensetive_res/new_dict.txt", ptr_pc))
{
    load_users_from_db();
    init_tree();

    worker = std::thread(&BotTelegram::check_message, this);
}

void BotTelegram::load_users_from_db()
{
    std::string conn = get_conn();
    PostgresDB db;
    std::vector<std::vector<std::string>> res;

    try{
        db.connect(conn);
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
        db.connect(conn);
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
    : flag(obj.flag.load()), worker(std::move(obj.worker)), offset(std::move(obj.offset)), searcher(std::move(obj.searcher)), observer(std::move(obj.observer))
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
    searcher = std::move(obj.searcher);
    observer = std::move(obj.observer);
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
        //auto ptr = TelegramSender::get_instance();
        // ptr->call(std::string(""), type_msg::read, std::string(offset));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // auto v = JsonReader::read("jq -r '.result[] | {text: .message.text, id:.message.from.id, update_id: .update_id}' ../res/result_"+ offset +".json", type_json::message);
        ts.read(offset);
        auto data = ts.get_response();
        if(data == "{\"ok\":true,\"result\":[]}") continue;
        nlohmann::json js = nlohmann::json::parse(data);
        
        std::string id = js["result"][0]["message"]["from"]["id"].dump();
        std::string full_message = js["result"][0]["message"]["text"];

        //id = id.substr(0, id.length()-1);

        if(users_with_keyboard.find(id) == users_with_keyboard.end()){
            send_main_keyboard(id);
            users_with_keyboard.insert(id);
        }

        auto waiting = MachingState.get_waiting(id);
        if(waiting != UserStateMaching::UserAction::NONE){
            if(waiting == UserStateMaching::UserAction::ADD_CARD)
                command_add_card(std::string(id), std::string(full_message));
            else if(waiting == UserStateMaching::UserAction::DEL_CARD)
                command_del_card(std::string(id), std::string(full_message));
            else if(waiting == UserStateMaching::UserAction::FORECAST)
                command_forecast(std::string(id), std::string(full_message));
            else if(waiting == UserStateMaching::UserAction::HAS_DISCOUNT)
                command_has_discount(std::string(id), std::string(full_message));
            MachingState.clear(id);
            offset_reload();
            continue;
        }

        if (full_message == "📋 Мои карточки") {
            command_my_cards(std::move(id));
        }
        else if (full_message == "💰 Статус скидок") {
            command_status(std::move(id));
        }
        else if (full_message == "🎯 Рекомендации") {
            command_recommendations(std::move(id));
        }
        else if (full_message == "➕ Добавить товар") {
            MachingState.set_waiting(id, UserStateMaching::UserAction::ADD_CARD);
            // ptr->call(id, type_msg::send, std::string("Введите название товара для добавления:"));
            ts.write(id, "Введите название товара для добавления:");
        }
        else if (full_message == "➖ Удалить товар") {
            MachingState.set_waiting(id, UserStateMaching::UserAction::DEL_CARD);
            // ptr->call(id, type_msg::send, std::string("Введите название товара для удаления:"));
            ts.write(id, "Введите название товара для удаления:");
        }
        else if (full_message == "📊 Прогноз") {
            MachingState.set_waiting(id, UserStateMaching::UserAction::FORECAST);
            // ptr->call(id, type_msg::send, std::string("Введите название товара для прогноза:"));
            ts.write(id, "Введите название товара для прогноза:");
        }
        else if(full_message == "❓ Узнать скидку"){
            MachingState.set_waiting(id, UserStateMaching::UserAction::HAS_DISCOUNT);
            // ptr->call(id, type_msg::send, std::string("Введите название товара для проверки скидки:"));
            ts.write(id, "Введите название товара для проверки скидки:");
        }
        else {
            // ptr->call(id, type_msg::send, std::string("Используйте кнопки меню"));
            ts.write(id, "Используйте кнопки меню");
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
            [{"text": "➕ Добавить товар"}, {"text": "➖ Удалить товар"}],
            [{"text": "📊 Прогноз"}, {"text": "❓ Узнать скидку"}]
        ],
        "resize_keyboard": true,
        "one_time_keyboard": false
    })";
    
    std::string message = 
        "🌟 Главное меню\n\n"
        "🔹 Нажимайте кнопки для команд\n"
        "🔹 Для добавления/удаления/прогноза введите название после нажатия";
    
    // auto ptr = TelegramSender::get_instance();
    // ptr->send_with_keyboard(id, message, keyboard_json);
    ts.send_keyboard(id, message, keyboard_json);
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
    TelegramUser user(id, ptr_pc);
    observer.add_user(id);
    this->add_user(std::move(user));

    send_main_keyboard(id);
    
    // auto ptr = TelegramSender::get_instance();
    // ptr->call(id, type_msg::send, std::string("Привет, теперь тебе доступен ряд команд для манипуляции с карточками\n"));
    ts.write(id, "Привет, теперь тебе доступен ряд команд для манипуляции с карточками");
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
    if(card.empty()){
        // auto ptr = TelegramSender::get_instance();
        // ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        ts.write(id, "Вы не ввели данные");
        offset_reload();
        return;
    }
    auto user = users->find(id);

    PostgresDB db;
    std::string conn = get_conn();

    std::vector<std::vector<std::string>> row;
    std::string last_sat = saturday_to_string(get_previous_or_current_saturday());
    //auto ptr = TelegramSender::get_instance();

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
            // ptr->call(id, type_msg::send, std::string("Скидка на " + true_card + " присутствует (данные взяты на число - " + last_sat + ")"));
            ts.write(id, "Скидка на " + true_card + " присутствует (данные взяты на число - " + last_sat + ")");
        } else {
            // ptr->call(id, type_msg::send, std::string("Скидка на " + true_card + " отсутствует (данные взяты на число - " + last_sat + ")"));
            ts.write(id, "Скидка на " + true_card + " отсутствует (данные взяты на число - " + last_sat + ")");
        }
    } else {
        // ptr->call(id, type_msg::send, std::string("Не удалось найти такую карточку в базе данных. Проверьте корректность названия карточки или же обратитесь к администратору\n"));
        ts.write(id, "Не удалось найти такую карточку в базе данных. Проверьте корректность названия карточки или же обратитесь к администратору");
    }
}

void BotTelegram::command_add_card(std::string&& id, std::string&& data)
{
    if(data.empty()){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        ts.write(id, "Вы не ввели данные");
        offset_reload();
        return;
    }
    auto user = users->find(id);
    
    PostgresDB db;
    std::string conn = get_conn();
    try{
        db.connect(conn);
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
    }

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
    //auto ptr = TelegramSender::get_instance();
    if(found)
        //ptr->call(id, type_msg::send, std::string("Карточка добавлена\n"));
        ts.write(id, "Карточка добавлена");
    else
        //ptr->call(id, type_msg::send, std::string("Не удалось найти такую карточку в базе данных. Проверьте корректность названия карточки или же обратитесь к администратору\n"));
        ts.write(id, "Не удалось найти такую карточку в базе данных. Проверьте корректность названия карточки или же обратитесь к администратору");
}

void BotTelegram::command_del_card(std::string&& id, std::string&& data)
{
    if(data.empty()){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        ts.write(id, "Вы не ввели данные");
        offset_reload();
        return;
    }
    auto user = users->find(id);
    user->second.del_product(data);
    //auto ptr = TelegramSender::get_instance();
    //ptr->call(id, type_msg::send, std::string("Карточка удалена\n"));
    ts.write(id, "Карточка удалена");
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
    //auto ptr = TelegramSender::get_instance();
    //ptr->call(id, type_msg::send, result);
    ts.write(id, result);
}

void BotTelegram::command_forecast(std::string&& id, std::string&& data)
{
    if(data.empty()){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(id, type_msg::send, std::string("Вы не ввели данные\n"));
        ts.write(id, "Вы не ввели данные");
        offset_reload();
        return;
    }

    auto load_cache = f_cache.get(data);
    if(load_cache != std::nullopt){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(id, type_msg::send, std::string("Вероятность скидки на данный товар: " + std::to_string(static_cast<int>(load_cache.value() * 100)) + "%"));
        ts.write(id, "Вероятность скидки на данный товар: " + std::to_string(static_cast<int>(load_cache.value() * 100)) + "%");
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
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{data});
    }
    if(query_result.empty()){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(id, type_msg::send, std::string("Данной карточки нет в базе данных или же ещё не было скидок на этот товар\n"));
        ts.write(id, "Данной карточки нет в базе данных или же ещё не было скидок на этот товар");
        offset_reload();
        return;
    }
    if(query_result.size() < 3){
        //auto ptr = TelegramSender::get_instance();
        //ptr->call(id, type_msg::send, std::string("Слишком мало данных для такой карточки\n"));
        ts.write(id, "Слишком мало данных для такой карточки");
        offset_reload();
        return;
    }

    auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{data});
    auto ymd = converte_string(first_date[0][0]);

    std::vector<int> sample;
    for(int i = 0; i < query_result.size();){
        if(converte_string(query_result[i][0]) == ymd){
            sample.push_back(1);
            i++;
        } else {
            sample.push_back(0);
        }
        std::chrono::sys_days date = std::chrono::sys_days{ymd};
        date += std::chrono::days{7};
        std::chrono::year_month_day n_ymd {date};
        ymd = n_ymd;
    }

    ModelSelector ms({TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL});
    ModelSelector::Result r;

    try{
        r = ms.select_best(sample);
    } catch (NoSuitableProbabilityException& e){
        r.best_probability = 0.0;
    }

    f_cache.set(data, r.best_probability);

    //auto ptr = TelegramSender::get_instance();
    //ptr->call(id, type_msg::send, std::string("Вероятность скидки на данный товар: " + std::to_string(static_cast<int>(r.best_probability * 100)) + "%"));
    ts.write(id, "Вероятность скидки на данный товар: " + std::to_string(static_cast<int>(r.best_probability * 100)) + "%");
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
    //auto ptr = TelegramSender::get_instance();
    //ptr->call(id, type_msg::send, result);
    ts.write(id, result);
}

void BotTelegram::reset_cache() noexcept
{
    f_cache.reset();
}