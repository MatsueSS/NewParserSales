#ifndef FEATURE_WEIGHT_SUM_H
#define FEATURE_WEIGHT_SUM_H

#include "Features_regression/FeatureRegression.h"

class FeatureWeightSum : public FeatureRegression<FeatureWeightSum> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        int weighted_sum = 0;
        for(int i = 0; i < window.size(); ++i){
            weighted_sum += window[i]*(i+1);
        }
        return weighted_sum;
    }

    static type_feature_regression name_impl() noexcept {
        return type_feature_regression::weighted_sum;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double min_w = 1e9, max_w = -1e9;
        for(int i = 0; i < train_size; ++i){
            max_w = std::max(max_w, sample[i]);
            min_w = std::min(min_w, sample[i]);
        }

        int n = sample.size();
        std::vector<double> norm_w(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_w-min_w > 1e-8) norm_w[i] = (sample[i]-min_w)/(max_w-min_w);
        }

        double raw_w = compute_impl(lasted_data), nw = 0.5;
        if(max_w-min_w > 1e-8) nw = (raw_w-min_w)/(max_w-min_w);

        return {norm_w, nw};
    }

};

#endif