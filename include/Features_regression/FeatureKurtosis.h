#ifndef FEATURE_KURTOSIS_H
#define FEATURE_KURTOSIS_H

#include "Feature.h"

class FeatureKurtosis : public Feature<FeatureKurtosis>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        if(window.size() < 4) return 0.0;

        double mean = 0.0;
        for(int val : window) mean += val;
        mean /= window.size();

        double variance = 0.0;
        for(int val : window) variance += (val-mean)*(val-mean);
        variance /= window.size();

        if(variance < 1e-8) return -3.0;

        double fourth_moment = 0.0;
        for(int val : window) fourth_moment += pow((val-mean)/sqrt(variance), 4);
        fourth_moment /= window.size();

        return fourth_moment - 3.0;
    }

    static type_feature name_impl() noexcept {
        return type_feature::kurtosis;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size){
        double max_kurtosis = -1e9;
        double min_kurtosis = 1e9;

        for(int i = 0; i < train_size; ++i){
            max_kurtosis = std::max(max_kurtosis, sample[i]);
            min_kurtosis = std::min(min_kurtosis, sample[i]);
        }

        int n = sample.size();
        std::vector<double> kurtosis_norm(n);
        for(int i = 0; i < n; ++i){
            if(max_kurtosis - min_kurtosis > 1e-8){
                kurtosis_norm[i] = (sample[i]-min_kurtosis)/(max_kurtosis-min_kurtosis);
            } else {
                kurtosis_norm[i] = 0.5;
            }
        }
        return kurtosis_norm;
    }

};

#endif