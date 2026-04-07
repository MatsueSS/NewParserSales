#include "GeometricModel.h"

#include <numeric>
#include <cmath>

#include <iostream>

GeometricModel::GeometricModel()
{
    name = TypeModel::GEOMETRIC_MODEL;
}

double GeometricModel::predict_probability(const std::vector<int>& sample) noexcept
{
    return forecast.geometric_probability(std::move(get_data(sample)), 0);
}

double GeometricModel::calculate_bic(const std::vector<int>& sample) noexcept
{
    auto v = get_data(sample);
    int S = std::accumulate(v.begin(), v.end(), 0);
    auto p = predict_probability(sample);
    return -2*(v.size() * log(p) + S * log(1-p)) + 1*log(v.size());
}

std::vector<int> GeometricModel::get_data(const std::vector<int>& sample) const noexcept
{
    std::vector<int> data;
    int count = 0;
    int i = 0;
    for(; i < sample.size(); ++i) if(sample[i++] == 1) break;
    for(; i < sample.size(); ++i) {
        if(sample[i] == 1){
            data.push_back(count+1);
            count = 0;
        } else {
            count++;
        }
    }
    return data;
}