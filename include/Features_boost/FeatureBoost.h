#ifndef FEATURE_BOOST_H
#define FEATURE_BOOST_H

#include "BoostHistory.h"

#include <cstdint>
#include <type_traits>

template<typename T>
concept ConceptBoostHistory = std::same_as<std::remove_cvref_t<T>, BoostHistory>;

enum class type_feature_boost{
    current_price = 1, change_price_1 = 2, change_price_pct_1 = 3, price_mean_4 = 4, price_std_4 = 5, price_min_4 = 6, price_max_4 = 7, price_mean_8 = 8, price_std_8 = 9, price_mean_12 = 10, price_std_12 = 11, prev_discount = 12, since_last_discount = 13, discount_count_4 = 14, discount_count_8 = 15, discount_count_12 = 16, discount_freq_4 = 17, discount_freq_8 = 18, discount_freq_12 = 19, last_discount_interval = 20, discount_prev_value = 21, week = 22, month = 23, quarter = 24
};

template<typename Derived>
class FeatureBoost{
public:
    static type_feature_boost name() noexcept {
        return Derived::name_impl();
    }

    template<ConceptBoostHistory T>
    static double compute(T&& sample) noexcept {
        return Derived::compute_impl(std::forward<T>(sample));
    }
};

#endif
