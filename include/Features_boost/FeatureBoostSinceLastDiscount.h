    #ifndef FEATURE_BOOST_SINCE_LAST_DISCOUNT_H
    #define FEATURE_BOOST_SINCE_LAST_DISCOUNT_H

    #include "FeatureBoost.h"

    class FeatureBoostSinceLastDiscount : public FeatureBoost<FeatureBoostSinceLastDiscount>{
    private:
        friend class FeatureBoost<FeatureBoostSinceLastDiscount>;

        static type_feature_boost name_impl() noexcept {
            return type_feature_boost::since_last_discount;
        }

        template<ConceptBoostHistory T>
        static double compute_impl(T&& sample) noexcept {
            int sz = sample.size();
            if(sz <= 1) return INT_MIN;
            int i = sz-2;
            while(i >= 0 && !sample[i].has_discount()) i--;
            return static_cast<double>(sz-i-1);
        }
    };

    #endif
