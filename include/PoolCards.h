#ifndef _POOL_CARDS_H_
#define _POOL_CARDS_H_

#include "Product.h"

#include <unordered_map>
#include <vector>
#include <shared_mutex>

class PoolCardsException : public std::exception{
protected:
    std::string msg;

public:
    PoolCardsException(std::string);
    PoolCardsException(const PoolCardsException&);

    const char * what() const noexcept override;
};

class BadTypePoolCardsException : public PoolCardsException{
public:
    BadTypePoolCardsException(std::string);
};

class PoolCards{
public:
    PoolCards();

    PoolCards(const PoolCards&) = default;
    PoolCards& operator=(const PoolCards&) = default;

    PoolCards(PoolCards&&) noexcept = default;
    PoolCards& operator=(PoolCards&&) noexcept = default;

    template<typename Data>
    uint32_t get_index(Data&& title) const noexcept;

    const Product& get_title(uint32_t id) const noexcept;

private:
    std::vector<Product> id_to_title;
    std::unordered_map<std::string, uint32_t> title_to_id;
    mutable std::shared_mutex mutex;

};

template<typename Data>
uint32_t PoolCards::get_index(Data&& title) const noexcept
{
    if constexpr(!std::is_same<std::decay_t<Data>, std::string>::value)
        throw BadTypePoolCardsException("Value type must be string");

    std::shared_lock<std::shared_mutex> lock(mutex);

    auto it = title_to_id.find(std::forward<Data>(title));
    return it == title_to_id.end() ? 0 : it->second;
}

#endif