#ifndef FEATURE_BOOST_PRICE_CHANGE_1_H
#define FEATURE_BOOST_PRICE_CHANGE_1_H

#include "FeatureBoost.h"

class FeatureBoostPriceChange1 : public FeatureBoost<FeatureBoostPriceChange1>{
private:
    friend class FeatureBoost<FeatureBoostPriceChange1>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::change_price_1;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        return static_cast<double>(sample[sz-1].get_price()) - static_cast<double>(sample[sz-2].get_price());
    }

};

#endif
