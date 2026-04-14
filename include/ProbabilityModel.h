#ifndef _PROBABILITY_MODEL_H_
#define _PROBABILITY_MODEL_H_

#include <vector>
#include <exception>
#include <string>

class ProbabilityModelException : public std::exception{
protected:
    std::string msg;

public:
    ProbabilityModelException(std::string msg) : msg(std::move(msg)) {}
    ProbabilityModelException(const ProbabilityModelException& obj) : msg(obj.msg) {}

    const char * what() const noexcept override { return msg.c_str(); }

};

class EmptySampleProbabilityModelException : public ProbabilityModelException{
public:
    EmptySampleProbabilityModelException(std::string msg) : ProbabilityModelException(std::move(msg)) {}

};

class InapplicabilityProbabilityModelException : public ProbabilityModelException{
public:
    InapplicabilityProbabilityModelException(std::string msg) : ProbabilityModelException(std::move(msg)) {}

};

class NoSuitableProbabilityException : public ProbabilityModelException{
public:
    NoSuitableProbabilityException(std::string msg) : ProbabilityModelException(std::move(msg)) {}
    
};

enum class TypeModel{
    GEOMETRIC_MODEL, MARKOV_CHAIN_1_MODEL, MARKOV_CHAIN_2_MODEL
};

class ProbabilityModel {
public:
    virtual ~ProbabilityModel() noexcept = default;
    virtual double predict_probability(const std::vector<int>&) = 0;
    virtual double calculate_bic(const std::vector<int>&) = 0;
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