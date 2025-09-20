#include "Reader.h"
#include "PostgresDB.h"
#include "PyLoader.h"
#include "json.hpp"

#define COUNT_RESOURSE 22

#include <fstream>
#include <iostream>

void Reader::make_note(std::string pq) {
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

std::string Reader::clean_price(const std::string& s) {
    std::string out;
    for (char c : s) {
        if ((c >= '0' && c <= '9') || c == '.')
            out += c;
    }
    return out.empty() ? "0" : out;
}