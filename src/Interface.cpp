#include "Interface.h"
#include "JsonReader.h"
#include "good_funcs.h"
#include "PyLoader.h"
#include "json.hpp"
#include "Matrix.h"
#include "PyAutoClickParser.h"
#include "PyHTMLParser.h"
#include "PostgresDB.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>

Interface::Interface(std::string str, RecType rectype, ProdType prodtype, TypeParses typeparser) : ptr(std::make_unique<BotTelegram>(std::move(str), rectype, prodtype)) 
{
    if(typeparser == TypeParses::PY_AUTOCLICK_PARSER){
        pr.set_strategy(std::move(std::make_unique<PyAutoClickParser>()));
    }
    else if(typeparser == TypeParses::PY_HTML_PARSER){
        pr.set_strategy(std::move(std::make_unique<PyHTMLParser>()));
    }
}

bool Interface::control_date() const 
{
    auto res = JsonReader::read(std::string("jq -r '.date' ../sensetive_res/products.json"), type_json::products);
    std::string date_str = res[0];
    std::istringstream ss(date_str);
    char delimiter;
    unsigned int year, month, day;
    ss >> year >> delimiter;
    if(delimiter != '-' || ss.fail()) return false;
    ss >> month >> delimiter;
    if(delimiter != '-' || ss.fail()) return false;
    ss >> day;
    if(ss.fail()) return false;

    auto ymd = std::chrono::year{(int)year}/std::chrono::month{month}/std::chrono::day{day};
    auto current_date = get_date_now();

    std::chrono::sys_days new_date_days = std::chrono::sys_days{ymd} + std::chrono::days{7};
    std::chrono::sys_days my_date = std::chrono::sys_days{current_date};
    
    return my_date >= new_date_days;
}

void Interface::start_process() const {
    bool flag = control_date();
    if(flag){
        auto cards = pr.fetch_product();

        auto date = save_in_bd(std::move(cards));
        PostgresDB db;
        db.connect(get_conn());

        auto discounts = db.fetch(std::string("SELECT title FROM cards WHERE date = $1 AND discount IS NOT NULL;"), std::vector<std::string>{date});

        for(const auto& obj : discounts){
            ptr->notify_all(obj[0]);
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

std::string Interface::save_in_bd(std::vector<ProductData>&& obj) const
{
    PostgresDB db;
    db.connect(get_conn());
    
    nlohmann::json data;
    std::ifstream file("../sensetive_res/products.json");
    data = nlohmann::json::parse(file);
    std::string date = data["date"];
    for(const auto& obj : data["products"]){
        if(obj.contains("discount")){
            db.execute(std::string("INSERT INTO cards (title, price, discount, date) VALUES ($1, $2, $3, $4);"), std::vector<std::string>{obj["title"], obj["price"], obj["discount"], date});
        }
        else{
            db.execute(std::string("INSERT INTO cards (title, price, date) VALUES ($1, $2, $3);"), std::vector<std::string>{obj["title"], obj["price"], date});
        }

        auto res = db.fetch(std::string("SELECT EXISTS (SELECT 1 FROM products WHERE title = $1);"), std::vector<std::string>{obj["title"]});
        if(res[0][0] == "f"){
            db.execute(std::string("INSERT INTO products (title) VALUES ($1)"), std::vector<std::string>{obj["title"]});
        }
    }
    return date;
}