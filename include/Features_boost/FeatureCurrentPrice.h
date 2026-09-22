#ifndef FEATURE_CURRENT_PRICE_H
#define FEATURE_CURRENT_PRICE_H

#include "FeatureBoost.h"

class FeatureCurrentPrice : public FeatureBoost<FeatureCurrentPrice>{
private:
    friend class FeatureBoost<FeatureCurrentPrice>;

    static type_feature_boost name_impl() noexcept{
        return type_feature_boost::current_price;
    }

    template<ConceptBoostHistory T>
    static std::uint32_t compute_impl(T&& sample) noexcept {
        return sample.back().get_price();
    }
};

#endif