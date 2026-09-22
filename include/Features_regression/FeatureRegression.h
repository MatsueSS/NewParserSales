#ifndef FEATURE_REGRESSION_H
#define FEATURE_REGRESSION_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <linear.h>

enum class type_feature_regression {
    pattern00 = 0, kurtosis = 1, autocorr_lag2 = 2, min_run = 3, max_run = 4, lag1 = 5, 
    lag2 = 6, transitions01 = 7, transitions10 = 8, transitions11 = 9, sum = 10, weighted_sum = 11,
    mode = 12, entropy = 13
};

struct result_normalize {
    std::vector<double> norm_sample;
    double last_norm;
};

template<typename Derived>
class FeatureRegression{
public:
    static double compute(const std::vector<int>& window) noexcept {
        return Derived::compute_impl(window);
    }

    static type_feature_regression name() noexcept {
        return Derived::name_impl();
    }

    static result_normalize normalize(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        return Derived::normalize_impl(sample, train_size, lasted_data);
    }

    static double pearson_correlation(const std::vector<double>& x, const std::vector<double>& y) noexcept {
        int n = x.size();

        double mean_x = 0.0, mean_y = 0.0;
        for(int i = 0; i < n; ++i){
            mean_x += x[i]; mean_y += y[i];
        }
        mean_x /= n; mean_y /= n;

        double cov = 0.0, var_x = 0.0, var_y = 0.0;
        for(int i = 0; i < n; ++i){
            double dx = x[i] - mean_x;
            double dy = y[i] - mean_y;
            cov += dx * dy;
            var_x += dx * dx;
            var_y += dy * dy;
        }

        if(var_x < 1e-8 || var_y < 1e-8) return 0.0;
        return cov / std::sqrt(var_x * var_y);
    }

};

#endif