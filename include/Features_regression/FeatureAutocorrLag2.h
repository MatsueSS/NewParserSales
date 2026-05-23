#ifndef FEATURE_AUTOCORR_LAG2_H
#define FEATURE_AUTOCORR_LAG2_H

#include "Features_regression/Feature.h"

class FeatureAutocorrLag2 : public Feature<FeatureAutocorrLag2>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept{
        if(window.size() < 4) return 0.0;

        int n = window.size() - 2;

        double mean1 = 0.0, mean2 = 0.0;
        for(int i = 0; i < n; ++i){
            mean1 += window[i];
            mean2 += window[i+2];
        }
        mean1/=n;
        mean2/=n;

        double cov = 0.0, var1 = 0.0, var2 = 0.0;
        for(int i = 0; i < n; ++i){
            cov += (window[i]-mean1)*(window[i+2]-mean2);
            var1 += (window[i]-mean1)*(window[i]-mean1);
            var2 += (window[i+2]-mean2)*(window[i+2]-mean2);
        }
        if(var1 < 1e-8 || var2 < 1e-8) return 0.0;

        return cov/std::sqrt(var1*var2);
    }

    static type_feature name_impl() noexcept {
        return type_feature::autocorr_lag2;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double max_autocorr = -1e9, min_autocorr = 1e9;
        for(int i = 0; i < train_size; ++i){
            max_autocorr = std::max(max_autocorr, sample[i]);
            min_autocorr = std::min(min_autocorr, sample[i]);
        }

        int n = sample.size();
        std::vector<double> autocorr_norm(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_autocorr - min_autocorr > 1e-8) autocorr_norm[i] = (sample[i]-min_autocorr)/(max_autocorr-min_autocorr);
        }

        double raw_autocorr = compute_impl(lasted_data), norm_autocorr = 0.5;
        if(max_autocorr - min_autocorr > 1e-8) norm_autocorr = (raw_autocorr-min_autocorr)/(max_autocorr-min_autocorr);

        return {autocorr_norm, norm_autocorr};
    }

};

#endif