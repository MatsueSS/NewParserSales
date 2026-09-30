#ifndef FEATURE_BOOST_DISCOUNT_PREV_VALUE_H
#define FEATURE_BOOST_DISCOUNT_PREV_VALUE_H

#include "FeatureBoost.h"

class FeatureBoostDiscountPrevValue : public FeatureBoost<FeatureBoostDiscountPrevValue>{
private:
    friend class FeatureBoost<FeatureBoostDiscountPrevValue>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_prev_value;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1 || !sample[sz-2].has_discount()) return INT_MIN;
        return static_cast<double>(*sample[sz-2].get_discount());
    }
};

#endif
