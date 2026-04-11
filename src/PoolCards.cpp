#include "PoolCards.h"

#include "PostgresDB.h"
#include "good_funcs.h"

PoolCardsException::PoolCardsException(std::string msg) : msg(std::move(msg)) {}
PoolCardsException::PoolCardsException(const PoolCardsException& obj) : msg(obj.msg) {}

const char* PoolCardsException::what() const noexcept { return msg.c_str(); }

BadTypePoolCardsException::BadTypePoolCardsException(std::string msg) : PoolCardsException(std::move(msg)) {}

PoolCards::PoolCards()
{
    PostgresDB db;
    db.connect(get_conn());

    std::vector<std::vector<std::string>> res = db.fetch(std::string("SELECT * FROM products;"), std::vector<std::string>{});
    int size = res.size();
    title_to_id.reserve(size + size/10);
    id_to_title.resize(size+1);
    for(const auto& obj : res){
        uint32_t card_id = std::stoi(obj[0]);
        Product product(obj[1], card_id);
        id_to_title[card_id] = product;
        title_to_id.emplace(std::make_pair(obj[1], card_id));
    }
}

const Product& PoolCards::get_title(uint32_t id) const noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    return id_to_title.at(id);
}