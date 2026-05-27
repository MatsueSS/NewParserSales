#ifndef FEATURE_LAG1_H
#define FEATURE_LAG1_H

#include "Features_regression/Feature.h"

class FeatureLag1 : public Feature<FeatureLag1> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept { return 1; }

    static type_feature name_impl() noexcept{
        return type_feature::lag1;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept{
        return {sample, static_cast<double>(lasted_data[lasted_data.size()-1])};
    }

};

#endif