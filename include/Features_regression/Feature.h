#ifndef FEATURE_FOR_REGRESSION_H
#define FEATURE_FOR_REGRESSION_H

#include <vector>
#include <algorithm>
#include <cmath>

enum class type_feature {
    pattern00, kurtosis, autocorr_lag2, min_run, max_run, lag1, 
    lag2, transitions01, transitions10, transitions11, sum, weighted_sum,
    mode, entropy
};

template<typename Derived>
class Feature{
public:
    static double compute(const std::vector<int>& window) noexcept {
        return Derived::compute_impl(window);
    }

    static type_feature name() noexcept {
        return Derived::name_impl();
    }

    static std::vector<double> normalize(const std::vector<double>& sample, int train_size) noexcept {
        return Derived::normalize_impl(sample, train_size);
    }

};

#endif