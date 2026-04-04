#include "Interface.h"
#include "JsonReader.h"
#include "good_funcs.h"
#include "PyLoader.h"
#include "Reader.h"
#include "json.hpp"
#include "Matrix.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>

Interface::Interface(std::string str, RecType rectype, ProdType prodtype) : ptr(std::make_unique<BotTelegram>(std::move(str), rectype, prodtype)) 
{}

bool Interface::control_date() const 
{
    auto res = JsonReader::read(std::string("jq -r '.date' ../sensetive_res/products_discount.json"), type_json::products);
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
        //PyLoader::load("bash -c 'python3 ../py_scripts/ex.py'");
        //PyLoader::load("bash -c 'python3 ../py_scripts/proxy_test.py'");
        //PyLoader::load("bash -c 'python3 ../py_scripts/2.py'");
        Reader reader;
        reader.make_note(get_conn(), "cards", "products");
        reader.make_a_json(get_conn());

        std::ifstream file("../sensetive_res/products_discount.json");
        nlohmann::json j = nlohmann::json::parse(file);

        for(const auto& obj : j["products"]){
            std::string title = obj["title"], discount = obj["discount"];
            ptr->notify_all(title);
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

// void Interface::temp_parse()
// {
//     std::vector<std::vector<std::string>> cards;
//     for(int i = 1; i <= 40; ++i){
//         std::ifstream file("../temps/temp_" + std::to_string(i) + ".json");

//         if(!file.is_open()) continue;

//         nlohmann::json data = nlohmann::json::parse(file);
//         auto products = data["products"];

//         for(const auto& product : products){
//             std::vector<std::string> t;
//             t.emplace_back(product["name"]);
//             const auto& prices = product["prices"];
//             t.emplace_back(prices["regular"]);
        
//             if (prices.contains("discount") && !prices["discount"].is_null()) {
//                 t.emplace_back(prices["discount"]);
//             }
//             cards.emplace_back(std::move(t));
//         }
//     }

//     nlohmann::json new_data;
//     new_data["date"] = get_date_str_now();

//     new_data["products"] = nlohmann::json::array();

//     std::unordered_map<std::string, card> hmap;

//     for(const auto& obj : cards){
//         card c;
//         c.title = obj[0];
//         c.price = obj[1];
//         if(obj.size() == 3) c.discount = obj[2];
//         hmap.emplace(std::string(obj[0]), c);
//     }

//     for(const auto& vec : hmap){
//         nlohmann::json product;
//         product["title"] = vec.second.title;
//         product["price"] = vec.second.price;
//         product["discount"] = vec.second.discount;
//         new_data["products"].push_back(product);
//     }

//     std::ofstream file("../sensetive_res/products.json");
//     file << new_data.dump(4);
// }