#ifndef FEATURE_BOOST_DISCOUNT_FREQ_12_H
#define FEATURE_BOOST_DISCOUNT_FREQ_12_H

#include "FeatureBoostDiscountCount12.h"

class FeatureBoostDiscountFreq12 : public FeatureBoost<FeatureBoostDiscountFreq12>{
private:
    friend class FeatureBoost<FeatureBoostDiscountFreq12>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_freq_12;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        double temp = FeatureBoostDiscountCount12::compute(std::forward<T>(sample));
        return temp == INT_MIN ? temp : temp/12;
    }
};

#endif
