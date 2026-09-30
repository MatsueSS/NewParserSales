#ifndef FEATURE_BOOST_LAST_DISCOUNT_INTERVAL_H
#define FEATURE_BOOST_LAST_DISCOUNT_INTERVAL_H

#include "FeatureBoost.h"

class FeatureBoostLastDiscountInterval : public FeatureBoost<FeatureBoostLastDiscountInterval>{
private:
    friend FeatureBoost<FeatureBoostLastDiscountInterval>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::last_discount_interval;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        auto i = sz-1;
        while(i >= 0 && !sample[i].has_discount()) i--;
        auto fixed = i--;
        while(i >= 0 && !sample[i].has_discount()) i--;
        return i == -1 ? INT_MIN : fixed-i;
    }
};

#endif
