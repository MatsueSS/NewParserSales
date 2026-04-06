#ifndef _PROBABILITY_MODEL_H_
#define _PROBABILITY_MODEL_H_

#include <vector>

enum class TypeModel{
    GEOMETRIC_MODEL, MARKOV_CHAIN_1_MODEL, MARKOV_CHAIN_2_MODEL
};

class ProbabilityModel {
public:
    virtual ~ProbabilityModel() = default;
    virtual double predict_probability(const std::vector<int>&) noexcept = 0;
    virtual double calculate_bic(const std::vector<int>&) noexcept = 0;
    TypeModel get_name() const noexcept { return name; };

protected:
    TypeModel name;
    
};

#endif