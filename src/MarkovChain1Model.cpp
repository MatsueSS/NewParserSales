#include "MarkovChain1Model.h"

#include <cmath>
#include <numeric>

MarkovChain1Model::MarkovChain1Model()
{
    name = TypeModel::MARKOV_CHAIN_1_MODEL;
}

double MarkovChain1Model::predict_probability(const std::vector<int>& sample) noexcept
{
    build_transitions(sample);
    int last_result = sample.back();
    double total = transition_type[{last_result, 1}] + transition_type[{last_result, 0}];
    return total == 0 ? 0.5 : transition_type[{last_result, 1}]/total;
}

double MarkovChain1Model::calculate_bic(const std::vector<int>& sample) noexcept
{
    build_transitions(sample);
    int N = sample.size() - 1;
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
    return -2*lg + 2*log(N);
}

void MarkovChain1Model::build_transitions(const std::vector<int>& sample) noexcept
{
    for(int x = 0; x < 2; ++x){
        for(int y = 0; y < 2; ++y){
            transition_type[{x,y}] = 0;
        }
    }

    for(int i = 1; i < sample.size(); ++i){
        transition_type[std::make_pair(sample[i-1], sample[i])]++;
    }
}