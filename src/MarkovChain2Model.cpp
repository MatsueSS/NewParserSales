#include "MarkovChain2Model.h"

#include <cmath>
#include <numeric>

MarkovChain2Model::MarkovChain2Model()
{
    name = TypeModel::MARKOV_CHAIN_2_MODEL;
}

double MarkovChain2Model::predict_probability(const std::vector<int>& sample)
{
    if(sample.size() < 2) throw EmptySampleProbabilityModelException("sample must be have at least 2 element");
    build_transitions(sample);
    int prelast = sample[sample.size()-2], last = sample[sample.size()-1];
    int total = transition_type[{prelast, last, 0}] + transition_type[{prelast, last, 1}];
    if(total == 0) throw InapplicabilityProbabilityModelException("The model is not applicable for such a sample");
    return (transition_type[{prelast, last, 1}])/static_cast<double>(total);
}

double MarkovChain2Model::calculate_bic(const std::vector<int>& sample)
{
    build_transitions(sample);
    double lg = 0;
    int N = sample.size() - 2;
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
        if(sum == 0) continue;
        for(int i : l){
            if(i != 0) lg += i*log(i/(double)sum);
        }
    }
    if(N <= 0) N = 1;
    return -2*lg + 4*log(N);
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