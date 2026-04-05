#include "PyAutoClickParser.h"

#include "PyLoader.h"
#include "json.hpp"
#include "good_funcs.h"

#include <fstream>

std::vector<ProductData> PyAutoClickParser::parse_temps() const
{
    std::vector<ProductData> cards;
    for(int i = 1; i <= 40; ++i){
        std::ifstream file("../temps/temp_" + std::to_string(i) + ".json");

        if(!file.is_open()) continue;

        nlohmann::json data = nlohmann::json::parse(file);
        auto products = data["products"];
        std::string date = get_date_str_now();

        for(const auto& product : products){
            if(!product.contains("name")) continue;
            ProductData d;
            d.title = product["name"];
            const auto& prices = product["prices"];
            d.price = clean_price(prices["regular"]);
        
            if (prices.contains("discount") && !prices["discount"].is_null()) {
                d.discount = clean_price(prices["discount"]);
            }
            cards.emplace_back(std::move(d));
        }
    }
    return cards;
}

std::vector<ProductData> PyAutoClickParser::fetch_product() const
{
    PyLoader::load("bash -c 'python3 ../py_scripts/full_autonomic.py'");
    auto cards = parse_temps();

    nlohmann::json new_data;
    new_data["date"] = get_date_str_now();

    new_data["products"] = nlohmann::json::array();

    std::unordered_map<std::string, ProductData> hmap;

    for(const auto& obj : cards){
        hmap.emplace(obj.title, obj);
    }

    for(const auto& obj : hmap){
        nlohmann::json product;
        product["title"] = obj.second.title;
        product["price"] = obj.second.price;
        if(obj.second.discount) product["discount"] = obj.second.discount.value();
        new_data["products"].emplace_back(std::move(product));
    }

    std::ofstream file("../sensetive_res/products.json");
    file << new_data.dump(4);
    
    return cards;
}