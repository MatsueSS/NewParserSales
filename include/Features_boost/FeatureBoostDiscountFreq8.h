#ifndef FEATURE_BOOST_DISCOUNT_FREQ_8_H
#define FEATURE_BOOST_DISCOUNT_FREQ_8_H

#include "FeatureBoostDiscountCount8.h"

class FeatureBoostDiscountFreq8 : public FeatureBoost<FeatureBoostDiscountFreq8>{
private:
    friend class FeatureBoost<FeatureBoostDiscountFreq8>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::discount_freq_8;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept{
        double temp = FeatureBoostDiscountCount8::compute(std::forward<T>(sample));
        return temp == INT_MIN ? temp : temp/8;
    }
};

#endif
