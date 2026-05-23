#include "Models/GeometricModel.h"

#include <numeric>
#include <cmath>

#include <iostream>

GeometricModel::GeometricModel()
{
    name = TypeModel::GEOMETRIC_MODEL;
}

std::pair<double, double> GeometricModel::calculate_bic_with_prob(const std::vector<int>& sample)
{
    auto v = get_data(sample);
    int S = std::accumulate(v.begin(), v.end(), 0);
    int n = v.size();
    
    auto p = forecast.geometric_probability((v), 0);
    double log_likelihood = n * log(p) + (S - n) * (1-p != 0 ? log(1 - p) : 0.0);
    
    double N = sample.size();
    int k = 1;
    
    return {p, -2 * log_likelihood + k * log(N)};
}

std::vector<int> GeometricModel::get_data(const std::vector<int>& sample) const noexcept
{
    std::vector<int> data;
    int count = 0;
    int i = 0;
    for(; i < sample.size(); ++i) if(sample[i] == 1) break;
    i++;
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