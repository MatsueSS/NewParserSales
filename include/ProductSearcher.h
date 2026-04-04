#ifndef _PRODUCT_SEARCHER_H_
#define _PRODUCT_SEARCHER_H_

#include "Matcher.h"

#include <memory>

class ProductSearcher{
public:
    ProductSearcher() = default;

    ProductSearcher(std::unique_ptr<Matcher> strategy);

    ProductSearcher(const ProductSearcher&) = delete;
    ProductSearcher& operator=(const ProductSearcher&) = delete;

    ProductSearcher(ProductSearcher&&) noexcept;
    ProductSearcher& operator=(ProductSearcher&&) noexcept;

    void set_strategy(std::unique_ptr<Matcher> new_strategy) noexcept;

    template<typename Data>
    const std::vector<uint32_t>* search(Data&& query) const noexcept;

private:
    std::unique_ptr<Matcher> searcher;

};

template<typename Data>
const std::vector<uint32_t>* ProductSearcher::search(Data&& query) const noexcept
{
    return searcher->find_matches(std::forward<Data>(query));
}

#endif