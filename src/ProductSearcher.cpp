#include "ProductSearcher.h"

ProductSearcher::ProductSearcher(std::unique_ptr<Matcher> strategy) : searcher(std::move(strategy)) {}

void ProductSearcher::set_strategy(std::unique_ptr<Matcher> new_strategy) noexcept
{
    searcher = std::move(new_strategy);
}