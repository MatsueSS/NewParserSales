#ifndef _PRODUCT_RECOMMENDATIONS_H_
#define _PRODUCT_RECOMMENDATIONS_H_

#include "Recommendations.h"

#include <memory>

enum class RecType {
    MATRIX
};

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

    template<typename Data>
    void add_user(Data&& id);
    
    template<typename Data>
    void del_user(Data&& id);

    template<typename Data1, typename Data2>
    void add_card(Data1&& id, Data2&& card);

    template<typename Data1, typename Data2>
    void del_card(Data1&& id, Data2&& card);

private:
    std::unique_ptr<Recommendations> prRec;

};

template<typename Data>
void ProductRecommendations::add_user(Data&& id)
{
    prRec->add_user(std::forward<Data>(id));
}

template<typename Data>
void ProductRecommendations::del_user(Data&& id)
{
    prRec->del_user(std::forward<Data>(id));
}

template<typename Data1, typename Data2>
void ProductRecommendations::add_card(Data1&& id, Data2&& card)
{
    prRec->add_card(std::forward<Data1>(id), std::forward<Data2>(card));
}

template<typename Data1, typename Data2>
void ProductRecommendations::del_card(Data1&& id, Data2&& card)
{
    prRec->del_card(std::forward<Data1>(id), std::forward<Data2>(card));
}

template<typename Data>
std::vector<std::string> ProductRecommendations::recommendation(Data&& id) const
{
    return prRec->recommendation(std::forward<Data>(id));
}

#endif