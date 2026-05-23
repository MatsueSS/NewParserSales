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

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double max_entr = -1e9, min_entr = 1e9;
        for(int i = 0; i < train_size; ++i){
            max_entr = std::max(max_entr, sample[i]);
            min_entr = std::min(min_entr, sample[i]);
        }

        int n = sample.size();
        std::vector<double> entropy_norm(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_entr - min_entr > 1e-8) entropy_norm[i] = (sample[i]-min_entr)/(max_entr-min_entr);
        }

        double raw_entr = compute_impl(lasted_data), norm_entr = 0.5;
        if(max_entr - min_entr > 1e-8) norm_entr = (raw_entr-min_entr)/(max_entr-min_entr);

        return {entropy_norm, norm_entr};
    }

};

#endif