#ifndef FEATURE_BOOST_PRICE_MAX_4_H
#define FEATURE_BOOST_PRICE_MAX_4_H

#include "FeatureBoost.h"

class FeatureBoostPriceMax4 : public FeatureBoost<FeatureBoostPriceMax4>{
private:
    friend class FeatureBoost<FeatureBoostPriceMax4>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::price_max_4;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        auto cur = static_cast<double>(sample[sz-2].get_price());
        auto lower = std::max(0, sz-5);
        for(int i = sz-3; i >= lower; --i){
            cur = std::max(cur, static_cast<double>(sample[i].get_price()));
        }
        return cur;
    }
};

#endif
