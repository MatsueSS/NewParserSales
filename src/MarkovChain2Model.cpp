#include "MarkovChain2Model.h"

#include <cmath>

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
        double p1 = it->second;
        double p2 = (++it)->second;

        if(p1 != 0) lg += p1*log(p1/(double)(p1+p2));
        if(p2 != 0) lg += p2*log(p2/(double)(p1+p2));

        ++it;
    }
    return -2*lg + 4*log(sample.size()-2);
}

void MarkovChain2Model::build_transitions(const std::vector<int>& sample) noexcept
{
    for(int x = 0; x <= 1; ++x) {
        for(int y = 0; y <= 1; ++y) {
            for(int z = 0; z <= 1; ++z) {
                transition_type[{x, y, z}] = 0;
            }
        }
    }

    for(int i = 2; i < sample.size(); ++i){
        transition_type[{sample[i-2], sample[i-1], sample[i]}]++;
    }
}