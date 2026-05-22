#ifndef FEATURE_TRANSITIONS_01_H
#define FEATURE_TRANSITIONS_01_H

#include "Features_regression/Feature.h"

class FeatureTransitions01 : public Feature<FeatureTransitions01>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        if(window.size() < 2) return 0;

        int count = 0;
        for(int i = 1; i < window.size(); ++i){
            if(window[i] == 0 && window[i-1] == 1) count++;
        }

        return count;
    }

    static type_feature name_impl() noexcept {
        return type_feature::transitions01;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double max_t01 = INT32_MIN, min_t01 = INT32_MAX;
        for(int i = 0; i < train_size; ++i){
            max_t01 = std::max(max_t01, sample[i]);
            min_t01 = std::min(min_t01, sample[i]);
        }

        int n = sample.size();
        std::vector<double> trans01_norm(n);
        for(int i = 0; i < n; ++i){
            if(max_t01 - min_t01 > 1e-8){
                trans01_norm[i] = (sample[i]-min_t01)/(max_t01-min_t01);
            } else {
                trans01_norm[i] = 0.5;
            }
        }
        return trans01_norm;
    }
};

#endif