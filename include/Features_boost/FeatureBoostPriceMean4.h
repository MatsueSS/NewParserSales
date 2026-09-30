#ifndef FEATURE_BOOST_PRICE_MEAN_4_H
#define FEATURE_BOOST_PRICE_MEAN_4_H

#include "FeatureBoost.h"

class FeatureBoostPriceMean4 : public FeatureBoost<FeatureBoostPriceMean4>{
private:
    friend class FeatureBoost<FeatureBoostPriceMean4>;

    static type_feature_boost name_impl() noexcept{
        return type_feature_boost::price_mean_4;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept{
        int sz = sample.size();
        if(sz <= 1) return INT_MIN;
        double cur = 0;
        auto lower = std::max(0, sz-5);
        for(int i = sz-2; i >= lower; --i){
            cur += static_cast<double>(sample[i].get_price());
        }
        return cur/(sz-lower-1);
    }
};

#endif
