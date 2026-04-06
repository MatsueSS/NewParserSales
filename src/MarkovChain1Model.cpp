#include "MarkovChain1Model.h"

#include <cmath>

MarkovChain1Model::MarkovChain1Model()
{
    name = TypeModel::MARKOV_CHAIN_1_MODEL;
}

double MarkovChain1Model::predict_probability(const std::vector<int>& sample) noexcept
{
    build_transitions(sample);
    int last_result = sample.back();
    double r1 = 0.5, r2 = 0.5;
    if(transition_type[{0,1}] != 0) r1 = (transition_type[{0,1}])/(double)(transition_type[{0,0}] + transition_type[{0,1}]);
    if(transition_type[{1,1}] != 0) r2 = (transition_type[{1,1}])/(double)(transition_type[{1,0}]+transition_type[{1,1}]);
    return last_result == 0 ? r1 : r2;
}

double MarkovChain1Model::calculate_bic(const std::vector<int>& sample) noexcept
{
    build_transitions(sample);
    int N = sample.size() - 1;
    double lg = 0;
    auto it = transition_type.begin();
    while(it != transition_type.end()){
        auto p1 = it->second;
        auto p2 = (++it)->second;

        if(p1 != 0) lg += p1*log(p1/(double)(p1+p2));
        if(p2 != 0) lg += p2*log(p2/(double)(p1+p2));

        ++it;
    }
    return -2*lg + 2*log(N);
}

void MarkovChain1Model::build_transitions(const std::vector<int>& sample) noexcept
{
    for(int x = 0; x <= 1; ++x){
        for(int y = 0; y <= 1; ++y){
            transition_type[{x,y}] = 0;
        }
    }

    for(int i = 1; i < sample.size(); ++i){
        transition_type[std::make_pair(sample[i-1], sample[i])]++;
    }
}