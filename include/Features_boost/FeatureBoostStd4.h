#ifndef FEATURE_BOOST_STD_4_H
#define FEATURE_BOOST_STD_4_H

#include "FeatureBoost.h"

#include <cmath>

class FeatureBoostStd4 : public FeatureBoost<FeatureBoostStd4>{
private:
    friend class FeatureBoost<FeatureBoostStd4>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::price_std_4;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        int sz = sample.size();
        if(sz <= 2) return INT_MIN;
        auto lower = std::max(0, sz-5);
        double mean = 0;
        for(int i = sz-2; i >= lower; --i){
            mean += static_cast<double>(sample[i].get_price());
        }
        mean /= (sz-lower-1);
        double disp = 0;
        for(int i = sz-2; i >= lower; --i){
            double temp = static_cast<double>(sample[i].get_price())-mean;
            disp += temp*temp;
        }
        disp /= (sz-lower-2);
        return std::sqrt(disp);
    }
};

#endif
