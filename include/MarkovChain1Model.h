#ifndef _MARKOV_CHAIN_1_MODEL_H_
#define _MARKOV_CHAIN_1_MODEL_H_

#include "ProbabilityModel.h"

#include <map>

class MarkovChain1Model : public ProbabilityModel{
public:
    MarkovChain1Model();

    virtual double predict_probability(const std::vector<int>&) noexcept override;
    virtual double calculate_bic(const std::vector<int>&) noexcept override;

private:
    std::map<std::pair<int, int>, int> transition_type;

    void build_transitions(const std::vector<int>&) noexcept;

};

#endif