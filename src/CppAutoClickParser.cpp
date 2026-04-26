#include "CppAutoClickParser.h"

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

void CppAutoClickParser::pull_json(std::vector<ProductData>& pd, const std::string& url) const
{
    system(("firefox --new-tab \"" + url + "\"").c_str());
    std::this_thread::sleep_for(std::chrono::seconds(7));
    system("ydotool mousemove 80 75");
    system("ydotool click 0xC0");
    system("ydotool mousemove 835 25");
    system("ydotool click 0xC0");

    nlohmann::json data = nlohmann::json::parse(get_clipboard_content());

    auto products = data["products"];

    for(const auto& product : products){
        if(!product.contains("name")) continue;
        ProductData d;
        d.title = product["name"];
        const auto& prices = product["prices"];
        d.price = clean_price(prices["regular"]);
    
        if (prices.contains("discount") && !prices["discount"].is_null()) {
            d.discount = clean_price(prices["discount"]);
        }
        pd.emplace_back(std::move(d));
    }
}

std::vector<ProductData> CppAutoClickParser::fetch_product() const
{
    std::vector<std::string> urls = {
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13057/products?mode=delivery&include_restrict=true&limit=350&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13055/products?mode=delivery&include_restrict=true&limit=250&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12886/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13093/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13094/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13095/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13096/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13097/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13098/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12888/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12889/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12890/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13070/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13071/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13072/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13073/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13074/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13075/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13076/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12901/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12903/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12904/products?mode=delivery&include_restrict=true&limit=499&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12905/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12906/products?mode=delivery&include_restrict=true&limit=499&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12907/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12908/products?mode=delivery&include_restrict=true&limit=499&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C12909/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13056/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13058/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13059/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13060/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13061/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13062/products?mode=delivery&include_restrict=true&limit=400&offset=0",
        "https://5d.5ka.ru/api/catalog/v2/stores/39KT/categories/251C13063/products?mode=delivery&include_restrict=true&limit=400&offset=0"
    };

    std::vector<ProductData> data;
    for(const auto& obj : urls){
        pull_json(data, obj);
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