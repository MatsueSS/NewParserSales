#ifndef FEATURE_BOOST_DISCOUNT_COUNT_4_H
#define FEATURE_BOOST_DISCOUNT_COUNT_4_H

#include "FeatureBoost.h"

class FeatureBoostDiscountCount4 : public FeatureBoost<FeatureBoostDiscountCount4>{
private:
    friend class FeatureBoost<FeatureBoostDiscountCount4>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_count_4;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        auto lower = std::max(0, sz-5);
        double ans = 0;
        for(int i = sz-2; i >= lower; --i){
            if(sample[i].has_discount()) ans++;
        }
        return ans;
    }
};

#endif
