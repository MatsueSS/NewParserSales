#ifndef _PROBABILITY_MODEL_H_
#define _PROBABILITY_MODEL_H_

#include <vector>

class ProbabilityModel {
public:
    virtual ~ProbabilityModel() = default;
    virtual double predict_probability(const std::vector<int>&) const noexcept = 0;
    virtual double calculate_bic(const std::vector<int>&) const noexcept = 0;
    
};

#endif