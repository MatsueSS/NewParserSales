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
    int find_max(const std::vector<int>& sample){
        int max_val = -1;
        for(int i : sample) max_val = std::max(max_val, i);
        return max_val+1;
    }

protected:
    TypeModel name;
    
};

#endif