#ifndef FEATURE_MODE_H
#define FEATURE_MODE_H

#include "Features_regression/FeatureRegression.h"

class FeatureMode : public FeatureRegression<FeatureMode> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        int sum = accumulate(window.begin(), window.end(), 0);
        return sum > window.size()/2;
    }

    static type_feature_regression name_impl() noexcept {
        return type_feature_regression::mode;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double max_max_run = -1e9, min_max_run = 1e9;
        for(int i = 0; i < train_size; ++i){
            max_max_run = std::max(max_max_run, sample[i]);
            min_max_run = std::min(min_max_run, sample[i]);
        }

        int n = sample.size();
        std::vector<double> max_run_norm(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_max_run - min_max_run > 1e-8) max_run_norm[i] = (sample[i]-min_max_run)/(max_max_run-min_max_run);
        }

        double raw_mr = compute_impl(lasted_data), norm_mr = 0.5;
        if(max_max_run - min_max_run > 1e-8) norm_mr = (raw_mr-min_max_run)/(max_max_run-min_max_run);

        return {max_run_norm, norm_mr};
    }

};

#endif