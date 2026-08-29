#ifndef MARKOV_CHAIN_1_MODEL_H
#define MARKOV_CHAIN_1_MODEL_H

#include "Models/ProbabilityModel.h"

#include <map>

class MarkovChain1Model : public ProbabilityModel{
public:
    MarkovChain1Model();

    virtual std::pair<double, double> calculate_bic_with_prob(const std::vector<int>&) override;

    virtual void change_regular(double C) noexcept override {}

private:
    std::map<std::pair<int, int>, int> transition_type;

    void build_transitions(const std::vector<int>&) noexcept;
    double predict_probability(const std::vector<int>&);

};

#endif // MARKOV_CHAIN_1_MODEL_H