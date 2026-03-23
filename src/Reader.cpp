#include "Reader.h"
#include "PostgresDB.h"
#include "PyLoader.h"
#include "json.hpp"
#include "good_funcs.h"

#include <fstream>
#include <iostream>

void Reader::make_note(const std::string& pq, const std::string& name_table, const std::string& file_name) {
    PostgresDB db;
    db.connect(pq);
    // PyLoader::load("bash -c 'python3 ../py_scripts/2.py'");
    
    nlohmann::json data;
    std::ifstream file("../sensetive_res/" + file_name + ".json");
    data = nlohmann::json::parse(file);
    std::string date = data["date"];
    for(const auto& obj : data["products"]){
        if(!obj.contains("title"))
            continue;
        std::string title = obj["title"];
        std::string price = obj["price"];
        price = clean_price(price);
        if(obj.contains("discount")){
            std::string discount = obj["discount"];
            discount = clean_price(discount);
            db.execute(std::string("INSERT INTO ") + name_table + std::string(" (title, price, discount, date) VALUES ($1, $2, $3, $4);"), std::vector<std::string>{title, price, discount, date});
        }
        else{
            db.execute(std::string("INSERT INTO ") + name_table + std::string(" (title, price, date) VALUES ($1, $2, $3);"), std::vector<std::string>{title, price, date});
        }

        auto res = db.fetch(std::string("SELECT EXISTS (SELECT 1 FROM products WHERE title = $1);"), std::vector<std::string>{title});
        if(res[0][0] == "f"){
            db.execute(std::string("INSERT INTO products (title) VALUES ($1)"), std::vector<std::string>{title});
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

    std::unordered_map<std::string, card> map;

    for(const auto& obj : cards){
        card c;
        c.title = obj[0];
        c.price = obj[1];
        c.discount = obj[2];
        map.insert({obj[0], c});
    }

    for(const auto& vec : map){
        nlohmann::json product;
        product["title"] = vec.second.title;
        product["price"] = vec.second.price;
        product["discount"] = vec.second.discount;
        new_data["products"].push_back(product);
    }

    std::ofstream file("../sensetive_res/products_discount.json");
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