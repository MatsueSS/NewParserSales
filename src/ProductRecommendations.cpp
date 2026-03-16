#include "ProductRecommendations.h"

ProductRecommendations::ProductRecommendations(std::unique_ptr<Recommendations> ptr) : prRec(std::move(ptr)) {}

ProductRecommendations::ProductRecommendations(ProductRecommendations&& obj) noexcept : prRec(std::move(obj.prRec)) {}

void ProductRecommendations::set_strategy(std::unique_ptr<Recommendations> new_strategy) noexcept
{
    prRec = std::move(new_strategy);
}

ProductRecommendations& ProductRecommendations::operator=(ProductRecommendations&& obj) noexcept
{
    prRec = std::move(obj.prRec);
    return *this;
}