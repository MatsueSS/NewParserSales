#include "Reader.h"
#include "PostgresDB.h"
#include "PyLoader.h"
#include "json.hpp"
#include "good_funcs.h"

#define COUNT_RESOURSE 22

#include <fstream>
#include <iostream>

void Reader::make_note(const std::string& pq) {
    PostgresDB db;
    db.connect(pq);
    PyLoader::load("bash -c 'python3 ../py_scripts/2.py'");
    
    for(int i = 1; i <= COUNT_RESOURSE; ++i){
        nlohmann::json data;
        std::ifstream file("../res/products_" + std::to_string(i) + ".json");
        data = nlohmann::json::parse(file);
        std::string date = data["date"];
        for(const auto& obj : data["products"]){
            std::string title = obj["title"];
            if(obj.contains("price")){
                std::string price = obj["price"];
                std::string discount = obj["discount"];
                price = clean_price(price);
                discount = clean_price(discount);
                db.execute(std::string("INSERT INTO cards (title, price, discount, \"date\") VALUES ($1, $2, $3, $4);"), std::vector<std::string>{title, price, discount, date});
            }
            else{
                std::string price = obj["discount"];
                price = clean_price(price);
                db.execute(std::string("INSERT INTO cards (title, price, \"date\") VALUES ($1, $2, $3)"), std::vector<std::string>{title, price, date});
            }
        }
    }
}

void Reader::make_a_json(const std::string& str){
    nlohmann::json new_data;
    new_data["date"] = get_date_str_now();

    PostgresDB db;
    db.connect(str);

    auto cards = db.fetch(std::string("SELECT title, price, discount, date FROM cards WHERE discount IS NOT NULL AND date >= CURRENT_DATE - INTERVAL '7 days'"), std::vector<std::string>{});

    new_data["products"] = nlohmann::json::array(); // создаём массив один раз

    for(const auto& vec : cards){
        nlohmann::json product;
        product["title"] = vec[0];
        product["price"] = vec[1];
        product["discount"] = vec[2];
        new_data["products"].push_back(product);
    }

    std::ofstream file("../res/products_discount.json");
    file << new_data.dump(4);
}


std::string Reader::clean_price(const std::string& s) {
    std::string out;
    for (char c : s) {
        if ((c >= '0' && c <= '9') || c == '.')
            out += c;
    }
    return out.empty() ? "0" : out;
}