#ifndef MARKOV_CHAIN_2_MODEL_H
#define MARKOV_CHAIN_2_MODEL_H

#include "Models/ProbabilityModel.h"

#include <map>
#include <tuple>

class MarkovChain2Model : public ProbabilityModel {
public:
    MarkovChain2Model();

    virtual std::pair<double, double> calculate_bic_with_prob(const std::vector<int>&) override;

    virtual void change_regular(double C) noexcept override {}

private:
    std::map<std::tuple<int, int, int>, int> transition_type;

    void build_transitions(const std::vector<int>&) noexcept;
    double predict_probability(const std::vector<int>&);

};

#endif // MARKOV_CHAIN_2_MODEL_H