#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include "Feature.h"

template<typename ...Features>
class FeatureExtractor{
public:
    static std::vector<double> extract(const std::vector<int>& window) noexcept {
        std::vector<double> result;

        ((result.push_back(Features::compute(window))), ...);

        return result;
    }

    static std::vector<type_feature> get_names() {
        std::vector<type_feature> names;
        (names.push_back(Features::name()), ...);
        return names;
    }

    static int count() {
        return sizeof...(Features);
    }

    static std::vector<std::vector<double>> normalize(const std::vector<double>& sample, int train_size) noexcept {
        std::vector<std::vector<double>> all_norm;
        (all_norm.emplace_back(std::move(Features::normalize(sample, train_size))), ...);
        return all_norm;
    }

};

#endif