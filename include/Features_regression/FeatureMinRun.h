#ifndef FEATURE_MIN_RUN_H
#define FEATURE_MIN_RUN_H

#include "Features_regression/Feature.h"

class FeatureMinRun : public Feature<FeatureMinRun> {
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

        int min_run = runs[0];
        for(int r : runs) min_run = std::min(min_run, r);
        return min_run;
    }

    static type_feature name_impl() noexcept {
        return type_feature::min_run;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        double max_min_run = -1e9, min_min_run = 1e9;
        for(int i = 0; i < train_size; ++i){
            max_min_run = std::max(max_min_run, sample[i]);
            min_min_run = std::min(min_min_run, sample[i]);
        }

        int n = sample.size();
        std::vector<double> min_run_norm(n, 0.5);
        for(int i = 0; i < n; ++i){
            if(max_min_run - min_min_run > 1e-8) min_run_norm[i] = (sample[i]-min_min_run)/(max_min_run-min_min_run);
        }

        double raw_mr = compute_impl(lasted_data), norm_mr = 0.5;
        if(max_min_run - min_min_run > 1e-8) norm_mr = (raw_mr-min_min_run)/(max_min_run-min_min_run);

        return {min_run_norm, norm_mr};
    }

};

#endif