#ifndef FEATURE_MODE_H
#define FEATURE_MODE_H

#include "Features_regression/Feature.h"

class FeatureMode : public Feature<FeatureMode> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept {
        int sum = accumulate(window.begin(), window.end(), 0);
        return sum > window.size()/2;
    }

    static type_feature name_impl() noexcept {
        return type_feature::mode;
    }

    static std::vector<double> normalize_impl(const std::vector<double>& sample, int train_size) noexcept {
        return sample;
    }

};

#endif