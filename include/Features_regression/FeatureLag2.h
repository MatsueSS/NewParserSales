#ifndef FEATURE_LAG2_H
#define FEATURE_LAG2_H

#include "Features_regression/FeatureRegression.h"

class FeatureLag2 : public FeatureRegression<FeatureLag2> {
public:
    static double compute_impl(const std::vector<int>& window) noexcept { return 1; }

    static type_feature_regression name_impl() noexcept{
        return type_feature_regression::lag2;
    }

    static result_normalize normalize_impl(const std::vector<double>& sample, int train_size, const std::vector<int>& lasted_data) noexcept{
        return {sample, static_cast<double>(lasted_data[lasted_data.size()-1])};
    }

};

#endif