#ifndef FEATURE_BOOST_DISCOUNT_COUNT_12_H
#define FEATURE_BOOST_DISCOUNT_COUNT_12_H

#include "FeatureBoost.h"

class FeatureBoostDiscountCount12 : public FeatureBoost<FeatureBoostDiscountCount12>{
private:
    friend class FeatureBoost<FeatureBoostDiscountCount12>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_count_12;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        auto lower = std::max(0, sz-13);
        double ans = 0;
        for(int i = sz-2; i >= lower; --i){
            if(sample[i].has_discount()) ans++;
        }
        return ans;
    }
};

#endif
