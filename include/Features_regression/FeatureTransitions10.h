#ifndef FEATURE_TRANSITIONS_10_H
#define FEATURE_TRANSITIONS_10_H

#include "Features_regression/FeatureRegression.h"

class FeatureTransitions10 : public FeatureRegression<FeatureTransitions10> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        if(window.size() < 2) return 0;

        int count = 0;
        for(int i = 1; i < window.size(); ++i){
            if(window[i] == 1 && window[i-1] == 0) count++;
        }

        return count;
    }

    static type_feature_regression name_impl() noexcept {
        return type_feature_regression::transitions10;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double min_trans = 1e9, max_trans = -1e9;
        for(int i = 0; i < train_size; ++i){
            max_trans = std::max(max_trans, sample[i]);
            min_trans = std::min(min_trans, sample[i]);
        }

        int n = sample.size();
        std::vector<double> norm_trans(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_trans-min_trans > 1e-8) norm_trans[i] = (sample[i]-min_trans)/(max_trans-min_trans);
        }

        double raw_trans10 = compute_impl(lasted_data), norm_trans10 = 0.5;
        if(max_trans-min_trans) norm_trans10 = (raw_trans10-min_trans)/(max_trans-min_trans);

        return {norm_trans, norm_trans10};
    }

};

#endif