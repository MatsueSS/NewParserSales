#ifndef FEATURE_BOOST_PRICE_CHANGE_1_H
#define FEATURE_BOOST_PRICE_CHANGE_1_H

#include "FeatureBoost.h"

class FeateruBoostPriceChange1 : public FeatureBoost<FeateruBoostPriceChange1>{
private:
    friend class FeatureBoost<FeateruBoostPriceChange1>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::change_price_1;
    }

    template<ConceptBoostHistory T>
    static std::int32_t compute_impl(T&& sample) noexcept {
        auto sz = sample.size();
        if(sz == 1) return 0;
        return static_cast<std::int32_t>(sample[sz-1].get_price()) - static_cast<std::int32_t>(sample[sz-2].get_price());
    }

};

#endif