#ifndef FEATURE_BOOST_DISCOUNT_COUNT_8_H
#define FEATURE_BOOST_DISCOUNT_COUNT_8_H

#include "FeatureBoost.h"

class FeatureBoostDiscountCount8 : public FeatureBoost<FeatureBoostDiscountCount8>{
private:
    friend class FeatureBoost<FeatureBoostDiscountCount8>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_count_8;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        auto lower = std::max(0, sz-9);
        double ans = 0;
        for(int i = sz-2; i >= lower; --i){
            if(sample[i].has_discount()) ans++;
        }
        return ans;
    }
};

#endif
