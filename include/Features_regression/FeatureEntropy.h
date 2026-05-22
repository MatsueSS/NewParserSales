#ifndef FEATURE_ENTROPY_H
#define FEATURE_ENTROPY_H

#include "Features_regression/Feature.h"

class FeatureEntropy : public Feature<FeatureEntropy> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        double entropy = 0;
        double p1 = accumulate(window.begin(), window.end(), 0) / (double)window.size();
        double p0 = 1 - p1;
        if(p0 > 0) entropy -= p0 * log(p0);
        if(p1 > 0) entropy -= p1 * log(p1);
        return entropy;
    }

    static type_feature name_impl() noexcept {
        return type_feature::entropy;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double max_entr = INT32_MIN, min_entr = INT32_MAX;
        for(int i = 0; i < train_size; ++i){
            max_entr = std::max(max_entr, sample[i]);
            min_entr = std::min(min_entr, sample[i]);
        }

        int n = sample.size();
        std::vector<double> entropy_norm(n);
        for(int i = 0; i < n; ++i){
            if(max_entr - min_entr > 1e-8){
                entropy_norm[i] = (sample[i]-min_entr)/(max_entr-min_entr);
            } else {
                entropy_norm[i] = 0.5;
            }
        }
        return entropy_norm;
    }

};

#endif