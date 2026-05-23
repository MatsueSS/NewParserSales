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

    static std::vector<result_normalize> normalize(const std::vector<std::vector<double>>& sample, int train_size, const std::vector<int>& lasted_data) noexcept {
        std::vector<result_normalize> all_norm;
        all_norm.reserve(sizeof...(Features));

        [&all_norm, &sample, train_size, &lasted_data]<size_t... Is>(std::index_sequence<Is...>){
            ((all_norm.push_back(Features::normalize(sample[Is], train_size, lasted_data))), ...);
        }(std::index_sequence_for<Features...>{});

        return all_norm;
    }

};

#endif