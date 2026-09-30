#ifndef FEATURE_BOOST_PRICE_CHANGE_PCT_1_H
#define FEATURE_BOOST_PRICE_CHANGE_PCT_1_H

#include "FeatureBoost.h"

class FeatureBoostPriceChangePct1 : public FeatureBoost<FeatureBoostPriceChangePct1>{
private:
    friend class FeatureBoost<FeatureBoostPriceChangePct1>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::change_price_pct_1;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        auto prev = static_cast<double>(sample[sz-2].get_price());
        return (static_cast<double>(sample[sz-1].get_price()) - prev)/prev;
    }
};

#endif
