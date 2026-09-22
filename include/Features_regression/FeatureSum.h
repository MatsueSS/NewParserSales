#ifndef FEATURE_SUM_H
#define FEATURE_SUM_H

#include "Features_regression/FeatureRegression.h"

class FeatureSum : public FeatureRegression<FeatureSum>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        return std::accumulate(window.begin(), window.end(), 0);
    }

    static type_feature_regression name_impl() noexcept {
        return type_feature_regression::sum;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double min_sum = 1e9, max_sum = -1e9;
        for(int i = 0; i < train_size; ++i){
            max_sum = std::max(max_sum, sample[i]);
            min_sum = std::min(min_sum, sample[i]);
        }

        int n = sample.size();
        std::vector<double> norm_sum(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_sum - min_sum > 1e-8) norm_sum[i] = (sample[i]-min_sum)/(max_sum-min_sum);
        }

        double raw_s = compute_impl(lasted_data), norm_s = 0.5;
        if(max_sum - min_sum > 1e-8) norm_s = (raw_s-min_sum)/(max_sum-min_sum);

        return {norm_sum, norm_s};
    }

};

#endif