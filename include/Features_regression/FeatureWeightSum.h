#ifndef FEATURE_WEIGHT_SUM_H
#define FEATURE_WEIGHT_SUM_H

#include "Features_regression/Feature.h"

class FeatureWeightSum : public Feature<FeatureWeightSum> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        int weighted_sum = 0;
        for(int i = 0; i < window.size(); ++i){
            weighted_sum += window[i]*(i+1);
        }
        return weighted_sum;
    }

    static type_feature name_impl() noexcept {
        return type_feature::weighted_sum;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double min_w = INT32_MAX, max_w = INT32_MIN;
        for(int i = 0; i < train_size; ++i){
            max_w = std::max(max_w, sample[i]);
            min_w = std::min(min_w, sample[i]);
        }

        int n = sample.size();
        std::vector<double> norm_w(n);
        for(int i = 0; i < n; ++i){
            if(max_w-min_w > 1e-8){
                norm_w[i] = (sample[i]-min_w)/(max_w-min_w);
            } else {
                norm_w[i] = 0.5;
            }
        }
        return norm_w;
    }

};

#endif