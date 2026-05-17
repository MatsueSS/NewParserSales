#ifndef FEATURE_PATTERN_00_H
#define FEATURE_PATTERN_00_H

#include "Feature.h"

class FeaturePattern00 : public Feature<FeaturePattern00>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        int count00 = 0;
        for(int i = 0; i < window.size()-1; ++i){
            if(window[i] == 0 && window[i+1] == 0) count00++;
        }
        return count00;
    }

    static type_feature name_impl() noexcept {
        return type_feature::pattern00;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double max_pattern = INT32_MIN;
        double min_pattern = INT32_MAX;
        for(int i = 0; i < train_size; ++i){
            max_pattern = std::max(max_pattern, sample[i]);
            min_pattern = std::min(min_pattern, sample[i]);
        }

        int n = sample.size();
        std::vector<double> pattern00_norm(n);
        for(int i = 0; i < n; ++i){
            if(max_pattern - min_pattern > 1e-8){
                pattern00_norm[i] = (sample[i]-min_pattern)/(max_pattern-min_pattern);
            } else {
                pattern00_norm[i] = 0.5;
            }
        }
        return pattern00_norm;
    }
};

#endif