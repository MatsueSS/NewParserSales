#ifndef FEATURE_BOOST_DISCOUNT_FREQ_4_H
#define FEATURE_BOOST_DISCOUNT_FREQ_4_H

#include "FeatureBoostDiscountCount4.h"

class FeatureBoostDiscountFreq4 : public FeatureBoost<FeatureBoostDiscountFreq4>{
private:
    friend class FeatureBoost<FeatureBoostDiscountFreq4>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_freq_4;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        double temp = FeatureBoostDiscountCount4::compute(std::forward<T>(sample));
        return temp == INT_MIN ? temp : temp/4;
    }
};

#endif
