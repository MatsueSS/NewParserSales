#include "MarkovChain2Model.h"

#include <cmath>
#include <numeric>

MarkovChain2Model::MarkovChain2Model()
{
    name = TypeModel::MARKOV_CHAIN_2_MODEL;
}

double MarkovChain2Model::predict_probability(const std::vector<int>& sample) noexcept
{
    build_transitions(sample);
    int prelast = sample[sample.size()-2], last = sample[sample.size()-1];
    double total = transition_type[{prelast, last, 0}] + transition_type[{prelast, last, 1}];
    return total == 0 ? 0.5 : (transition_type[{prelast, last, 1}])/total;
}

double MarkovChain2Model::calculate_bic(const std::vector<int>& sample) noexcept
{
    build_transitions(sample);
    double lg = 0;
    auto it = transition_type.begin();
    while(it != transition_type.end()){
        std::vector<double> l;
        int count = 0;
        while(count < 2){
            l.push_back(it->second);
            count++;
            it++;
        }
        int sum = std::accumulate(l.begin(), l.end(), 0);
        for(int i : l){
            if(i != 0) lg += i*log(i/(double)sum);
        }
    }
    return -2*lg + 4*log(sample.size()-2);
}

void MarkovChain2Model::build_transitions(const std::vector<int>& sample) noexcept
{
    for(int x = 0; x < 2; ++x) {
        for(int y = 0; y < 2; ++y) {
            for(int z = 0; z < 2; ++z) {
                transition_type[{x, y, z}] = 0;
            }
        }
    }

    for(int i = 2; i < sample.size(); ++i){
        transition_type[{sample[i-2], sample[i-1], sample[i]}]++;
    }
}