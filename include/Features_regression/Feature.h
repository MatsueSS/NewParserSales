#ifndef FEATURE_FOR_REGRESSION_H
#define FEATURE_FOR_REGRESSION_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <linear.h>

enum class type_feature {
    pattern00, kurtosis, autocorr_lag2, min_run, max_run, lag1, 
    lag2, transitions01, transitions10, transitions11, sum, weighted_sum,
    mode, entropy
};

struct result_normalize {
    std::vector<double> norm_sample;
    double last_norm;
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

    static result_normalize normalize(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        return Derived::normalize_impl(sample, train_size, lasted_data);
    }

    static double calculate_bic(struct model* model_, const std::vector<std::vector<double>>& X, const std::vector<double>& y, int n_features, bool has_bias = false) noexcept {
        int n = X.size();
        int k = has_bias ? n_features + 1 : n_features;
        
        double log_likelihood = 0.0;
        const double epsilon = 1e-15;
        
        for (int i = 0; i < n; i++) {
            std::vector<feature_node> nodes(n_features + 1);
            for (int j = 0; j < n_features; j++) {
                nodes[j].index = j + 1;
                nodes[j].value = X[i][j];
            }
            nodes[n_features].index = -1;
            
            double probs[2];
            predict_probability(model_, nodes.data(), probs);
            
            double prob = (y[i] == 1.0) ? probs[1] : probs[0];
            prob = std::max(epsilon, std::min(1.0 - epsilon, prob));
            
            log_likelihood += log(prob);
        }
        
        return -2.0 * log_likelihood + k * log(n);
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