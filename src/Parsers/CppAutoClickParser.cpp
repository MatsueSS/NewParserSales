#include "Parsers/CppAutoClickParser.h"

#include "json.hpp"
#include "good_funcs.h"

#include <chrono>
#include <thread>
#include <fstream>

std::string CppAutoClickParser::get_clipboard_content() const
{
    std::string result;
    FILE* pipe = popen("xclip -selection clipboard -o", "r");
    if(!pipe) throw NotExistParserException("sudo apt install xclip");
    char buffer[128];
    while(fgets(buffer, sizeof(buffer), pipe) != nullptr){ result += buffer; }
    pclose(pipe);
    return result;
}

void CppAutoClickParser::pull_json(std::vector<ProductData>& pd, const std::string& url, int i) const
{
    system(("firefox --new-tab \"" + url + "\"").c_str());
    std::this_thread::sleep_for(std::chrono::seconds(7));
    system("ydotool mousemove 80 75");
    system("ydotool click 0xC0");
    system("ydotool mousemove 835 25");
    system("ydotool click 0xC0");
    // system(("google-chrome --new-tab \"" + url + "\"").c_str());
    // std::this_thread::sleep_for(std::chrono::seconds(7));   
    // system("ydotool mousemove 900 65");
    // system("ydotool click 0xC0");
    // system("ydotool mousemove 860 20");
    // system("ydotool click 0xC0");

    nlohmann::json data = nlohmann::json::parse(get_clipboard_content());

    auto products = data["products"];

    std::ofstream file("../res/jsons/" + std::to_string(i) + ".json");

    for(const auto& product : products){
        if(!product.contains("name")) continue;
        ProductData d;
        d.title = product["name"];
        const auto& prices = product["prices"];
        d.price = clean_price(prices["regular"]);

        file << d.title << '|' << d.price;
    
        if (prices.contains("discount") && !prices["discount"].is_null()) {
            d.discount = clean_price(prices["discount"]);
            file << '|' << *(d.discount);
        }

        file << '\n';

        pd.emplace_back(std::move(d));
    }
    
    file.close();
}

std::vector<ProductData> CppAutoClickParser::fetch_product() const
{
    std::vector<std::string> urls = {
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12884/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C51627/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C51941/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C51979/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C51985/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C51994/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52002/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        // "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52027/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52032/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52037/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12890/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12888/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13070/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13071/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13072/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13073/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13074/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13075/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13076/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12901/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52952/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52955/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52956/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52957/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52958/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52959/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52960/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52961/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52962/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C52970/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12904/products?mode=delivery&include_restrict=true&limit=499&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12905/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C56116/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C56117/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C56118/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C56119/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C56120/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12907/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C55984/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C55982/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C55985/products?mode=delivery&include_restrict=true&limit=400&offset=0"
    };

    int i = 8;
    std::vector<ProductData> data;
    for(const auto& obj : urls){
        pull_json(data, obj, ++i);
    }

    data.clear();
    i = 1;
    for(; i <= 41; ++i){
        std::ifstream file("../res/jsons/" + std::to_string(i) + ".json");
        std::string line;
        while(std::getline(file, line)){
            if(line.empty()) continue;

            ProductData product;
            std::istringstream ss(line);
            std::string token;

            std::vector<std::string> tokens;

            while(std::getline(ss, token, '|')){
                tokens.push_back(token);
            }

            product.title = tokens[0];
            product.price = tokens[1];
            if(tokens.size() == 3) product.discount = tokens[2];

            data.emplace_back(std::move(product));
        }
        file.close();
    }

    nlohmann::json new_data;
    new_data["date"] = get_date_str_now();

    new_data["products"] = nlohmann::json::array();

    std::unordered_map<std::string, ProductData> hmap;

    for(const auto& obj : data){
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

    return data;
}