#ifndef FEATURE_SUM_H
#define FEATURE_SUM_H

#include "Features_regression/Feature.h"

class FeatureSum : public Feature<FeatureSum>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        return std::accumulate(window.begin(), window.end(), 0);
    }

    static type_feature name_impl() noexcept {
        return type_feature::sum;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double min_sum = INT32_MAX, max_sum = INT32_MIN;
        for(int i = 0; i < train_size; ++i){
            max_sum = std::max(max_sum, sample[i]);
            min_sum = std::min(min_sum, sample[i]);
        }

        int n = sample.size();
        std::vector<double> norm_sum(n);
        for(int i = 0; i < n; ++i){
            if(max_sum-min_sum > 1e-8){
                norm_sum[i] = (sample[i]-min_sum)/(max_sum-min_sum);
            } else {
                norm_sum[i] = 0.5;
            }
        }
        return norm_sum;
    }

};

#endif