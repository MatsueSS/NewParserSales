#ifndef PRODUCT_RECOMMENDATIONS_H
#define PRODUCT_RECOMMENDATIONS_H

#include "Recommendations.h"

#include <memory>

class ProductRecommendations{
public:
    ProductRecommendations() = default;

    ProductRecommendations(std::unique_ptr<Recommendations>);

    ProductRecommendations(const ProductRecommendations&) = delete;
    ProductRecommendations& operator=(const ProductRecommendations&) = delete;

    ProductRecommendations(ProductRecommendations&&) noexcept;
    ProductRecommendations& operator=(ProductRecommendations&&) noexcept;

    void set_strategy(std::unique_ptr<Recommendations> new_strategy) noexcept;

    template<typename Data>
    std::vector<std::string> recommendation(Data&& id) const; 

private:
    std::unique_ptr<Recommendations> prRec;

};

template<typename Data>
std::vector<std::string> ProductRecommendations::recommendation(Data&& id) const
{
    return prRec->recommendation(std::forward<Data>(id));
}

#endif // PRODUCT_RECOMMENDATIONS_H