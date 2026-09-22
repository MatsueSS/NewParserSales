#ifndef FEATURE_PATTERN_00_H
#define FEATURE_PATTERN_00_H

#include "FeatureRegression.h"

class FeaturePattern00 : public FeatureRegression<FeaturePattern00>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        int count00 = 0;
        for(int i = 0; i < window.size()-1; ++i){
            if(window[i] == 0 && window[i+1] == 0) count00++;
        }
        return count00;
    }

    static type_feature_regression name_impl() noexcept {
        return type_feature_regression::pattern00;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double max_pattern = INT32_MIN;
        double min_pattern = INT32_MAX;
        for(int i = 0; i < train_size; ++i){
            max_pattern = std::max(max_pattern, sample[i]);
            min_pattern = std::min(min_pattern, sample[i]);
        }

        int n = sample.size();
        std::vector<double> pattern00_norm(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_pattern - min_pattern > 1e-8) pattern00_norm[i] = (sample[i]-min_pattern)/(max_pattern-min_pattern);
        }

        double raw_pattern = compute_impl(lasted_data), norm_pattern = 0.5;
        if(max_pattern - min_pattern > 1e-8) norm_pattern = (raw_pattern-min_pattern)/(max_pattern-min_pattern);

        return {pattern00_norm, norm_pattern};
    }

};

#endif