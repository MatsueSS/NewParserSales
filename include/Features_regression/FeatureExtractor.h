#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include "FeatureRegression.h"

template<typename ...FeaturesRegression>
class FeatureExtractor{
public:
    static std::vector<double> extract(const std::vector<int>& window) noexcept {
        std::vector<double> result;

        ((result.push_back(FeaturesRegression::compute(window))), ...);

        return result;
    }

    static std::vector<type_feature_regression> get_names() noexcept {
        std::vector<type_feature_regression> names;
        (names.push_back(FeaturesRegression::name()), ...);
        return names;
    }

    static int count() {
        return sizeof...(FeaturesRegression);
    }

    static std::vector<result_normalize> normalize(const std::vector<std::vector<double>>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        std::vector<result_normalize> all_norm;
        all_norm.reserve(sizeof...(FeaturesRegression));

        [&all_norm, &sample, train_size, &lasted_data]<size_t... Is>(std::index_sequence<Is...>){
            ((all_norm.push_back(FeaturesRegression::normalize(sample[Is], train_size, lasted_data))), ...);
        }(std::index_sequence_for<FeaturesRegression...>{});

        return all_norm;
    }

};

#endif
