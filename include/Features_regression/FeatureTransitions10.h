#ifndef FEATURE_TRANSITIONS_10_H
#define FEATURE_TRANSITIONS_10_H

#include "Features_regression/Feature.h"

class FeatureTransitions10 : public Feature<FeatureTransitions10> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        if(window.size() < 2) return 0;

        int count = 0;
        for(int i = 1; i < window.size(); ++i){
            if(window[i] == 1 && window[i-1] == 0) count++;
        }

        return count;
    }

    static type_feature name_impl() noexcept {
        return type_feature::transitions10;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double min_trans = INT32_MAX, max_trans = INT32_MIN;
        for(int i = 0; i < train_size; ++i){
            max_trans = std::max(max_trans, sample[i]);
            min_trans = std::min(min_trans, sample[i]);
        }

        int n = sample.size();
        std::vector<double> norm_trans(n);
        for(int i = 0; i < n; ++i){
            if(max_trans-min_trans > 1e-8){
                norm_trans[i] = (sample[i]-min_trans)/(max_trans-min_trans);
            } else {
                norm_trans[i] = 0.5;
            }
        }
        return norm_trans;
    }

};

#endif