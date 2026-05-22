#ifndef FEATURE_MAX_RUN_H
#define FEATURE_MAX_RUN_H

#include "Features_regression/Feature.h"

class FeatureMaxRun : public Feature<FeatureMaxRun>{
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        if(window.empty()) return 0;

        std::vector<int> runs;
        int current_run = 1;

        for(size_t i = 1; i < window.size(); ++i){
            if(window[i] == window[i-1]) current_run++;
            else {
                runs.push_back(current_run);
                current_run = 1;
            }
        }
        runs.push_back(current_run);

        int max_run = runs[0];
        for(int r : runs) max_run = std::max(max_run, r);
        return max_run;
    }

    static type_feature name_impl() noexcept {
        return type_feature::max_run;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        double max_max_run = INT32_MIN, min_max_run = INT32_MAX;
        for(int i = 0; i < train_size; ++i){
            max_max_run = std::max(max_max_run, sample[i]);
            min_max_run = std::min(min_max_run, sample[i]);
        }

        int n = sample.size();
        std::vector<double> max_run_norm(n);
        for(int i = 0; i < n; ++i){
            if(max_max_run - min_max_run > 1e-8){
                max_run_norm[i] = (sample[i]-min_max_run)/(max_max_run-min_max_run);
            } else {
                max_run_norm[i] = 0.5;
            }
        }
        return max_run_norm;
    }

};

#endif