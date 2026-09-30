#ifndef FEATURE_BOOST_QUARTER_H
#define FEATURE_BOOST_QUARTER_H

#include "FeatureBoost.h"

class FeatureBoostQuarter : public FeatureBoost<FeatureBoostQuarter>{
private:
    friend class FeatureBoost<FeatureBoostQuarter>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::quarter;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        if(sample.empty()) return INT_MIN;
        auto m = static_cast<unsigned>(sample.back().get_date().month());
        if(m <= 3) return 1;
        else if(m <= 6) return 2;
        else if(m <= 9) return 3;
        else return 4;
    }
};

#endif
