#ifndef FEATURE_BOOST_H
#define FEATURE_BOOST_H

#include "BoostHistory.h"

#include <cstdint>
#include <type_traits>

template<typename T>
concept ConceptBoostHistory = std::same_as<std::remove_cvref_t<T>, BoostHistory>;

enum class type_feature_boost{
    current_price = 1
};

template<typename Derived>
class FeatureBoost{
public:
    static type_feature_boost name() noexcept {
        return Derived::name_impl();
    }

    template<ConceptBoostHistory T>
    static std::uint32_t compute(T&& sample) noexcept {
        return Derived::compute_impl(std::forward<T>(sample));
    }
};

#endif