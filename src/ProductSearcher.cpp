#include "ProductSearcher.h"

ProductSearcher::ProductSearcher(std::unique_ptr<Matcher> strategy) : searcher(std::move(strategy)) {}

ProductSearcher::ProductSearcher(ProductSearcher&& obj) noexcept : searcher(std::move(obj.searcher)) {}

ProductSearcher& ProductSearcher::operator=(ProductSearcher&& obj) noexcept
{
    searcher = std::move(obj.searcher);
    return *this;
}

void ProductSearcher::set_strategy(std::unique_ptr<Matcher> new_strategy) noexcept
{
    searcher = std::move(new_strategy);
}