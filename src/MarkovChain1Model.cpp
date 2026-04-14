#include "MarkovChain1Model.h"

#include <cmath>
#include <numeric>

MarkovChain1Model::MarkovChain1Model()
{
    name = TypeModel::MARKOV_CHAIN_1_MODEL;
}

double MarkovChain1Model::predict_probability(const std::vector<int>& sample)
{
    if(sample.empty()) throw EmptySampleProbabilityModelException("sample must be have at least 1 element");
    build_transitions(sample);
    int last_result = sample.back();
    int total = transition_type[{last_result, 1}] + transition_type[{last_result, 0}];
    if(total == 0) throw InapplicabilityProbabilityModelException("The model is not applicable for such a sample");
    return transition_type[{last_result, 1}]/static_cast<double>(total);
}

double MarkovChain1Model::calculate_bic(const std::vector<int>& sample)
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
        if(sum == 0) continue;
        for(int i : l){
            if(i != 0) lg += i*log(i/static_cast<double>(sum));
        }
    }
    if(N <= 0) N = 1;
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