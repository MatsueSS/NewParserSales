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

    uint32_t get_index(const std::string& title) const noexcept;

    const Product& get_title(uint32_t id) const noexcept;

    void add_product(const std::string& title) noexcept;

private:
    std::vector<Product> id_to_title;
    std::unordered_map<std::string, uint32_t> title_to_id;
    mutable std::shared_mutex mutex;

};

#endif