#include "Parser.h"

#include "json.hpp"

#include <fstream>

std::vector<ProductData> Parser::parse_json(std::string filename) const
{
    std::ifstream file(filename);
    if(!file.is_open()) throw "";

    nlohmann::json data = nlohmann::json::parse(file);
    std::string date = data["date"];
    std::vector<ProductData> result;
    for(const auto& obj : data["products"]){
        if(!obj.contains("title")) continue;
        ProductData d;
        d.title = obj["title"];
        d.price = clean_price(obj["price"]);
        d.date = date;
        if(obj.contains("discount")) d.discount = clean_price(obj["discount"]);
        result.emplace_back(std::move(d));
    }
    return result;
}

std::string Parser::clean_price(const std::string& s) const noexcept
{
    auto pos = s.find('.');
    return pos == std::string::npos ? s : s.substr(0, pos);
}