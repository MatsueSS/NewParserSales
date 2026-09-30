#ifndef FEATURE_BOOST_MONTH_H
#define FEATURE_BOOST_MONTH_H

#include "FeatureBoost.h"

class FeatureBoostMonth : public FeatureBoost<FeatureBoostMonth>{
private:
    friend class FeatureBoost<FeatureBoostMonth>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::month;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        if(sample.empty()) return INT_MIN;
        return static_cast<double>(static_cast<unsigned>(sample.back().get_date().month()));
    }
};

#endif
