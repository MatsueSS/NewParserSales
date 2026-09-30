#ifndef FEATURE_BOOST_CURRENT_PRICE_H
#define FEATURE_BOOST_CURRENT_PRICE_H

#include "FeatureBoost.h"

class FeatureBoostCurrentPrice : public FeatureBoost<FeatureBoostCurrentPrice>{
private:
    friend class FeatureBoost<FeatureBoostCurrentPrice>;

    static type_feature_boost name_impl() noexcept{
        return type_feature_boost::current_price;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        if(sample.empty()) return INT_MIN;
        return static_cast<double>(sample.back().get_price());
    }
};

#endif
