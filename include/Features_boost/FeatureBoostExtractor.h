#ifndef FEATURE_BOOST_EXTRACTOR_H
#define FEATURE_BOOST_EXTRACTOR_H

#include "FeatureBoost.h"
#include "BoostHistory.h"

template<typename ...FeaturesBoost>
class FeatureBoostExtractor{
public:
    static std::vector<std::vector<float>> extract(const BoostHistory& sample) noexcept{
        std::vector<std::vector<float>> mtx;
        BoostHistory temp;
        for(int i = 0; i < sample.size(); ++i){
            std::vector<float> row;
            temp.add_product(sample[i]);
            ((row.push_back(FeaturesBoost::compute(temp))), ...);
            mtx.emplace_back(std::move(row));
        }
        return mtx;
    }

    static std::vector<type_feature_boost> get_names() noexcept {
        std::vector<type_feature_boost> names;
        ((names.push_back(FeaturesBoost::name())), ...);
        return names;
    }
};

#endif
