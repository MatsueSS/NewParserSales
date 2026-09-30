#ifndef FEATURE_BOOST_PRICE_MEAN_12_H
#define FEATURE_BOOST_PRICE_MEAN_12_H

#include "FeatureBoost.h"

class FeatureBoostPriceMean12 : public FeatureBoost<FeatureBoostPriceMean12>{
private:
    friend class FeatureBoost<FeatureBoostPriceMean12>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::price_mean_12;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        int lower = std::max(0, sz-13);
        double cur = 0;
        for(int i = sz-2; i >= lower; --i){
            cur += static_cast<double>(sample[i].get_price());
        }
        return cur/(sz-lower-1);
    }
};

#endif
