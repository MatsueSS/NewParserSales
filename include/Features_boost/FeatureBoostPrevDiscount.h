#ifndef FEATURE_BOOST_PREV_DISCOUNT_H
#define FEATURE_BOOST_PREV_DISCOUNT_H

#include "FeatureBoost.h"

class FeatureBoostPrevDiscount : public FeatureBoost<FeatureBoostPrevDiscount>{
private:
    friend class FeatureBoost<FeatureBoostPrevDiscount>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::prev_discount;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        return sample[sz-2].has_discount() ? 1 : 0;
    }
};

#endif
