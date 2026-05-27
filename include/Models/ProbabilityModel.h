#ifndef PROBABILITY_MODEL_H
#define PROBABILITY_MODEL_H

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

class BadTypeProbabilityModelException : public ProbabilityModelException{
public:
    BadTypeProbabilityModelException(std::string msg) : ProbabilityModelException(std::move(msg)) {}

};

enum class TypeModel{
    GEOMETRIC_MODEL, MARKOV_CHAIN_1_MODEL, MARKOV_CHAIN_2_MODEL, LOGISTIC_REGRESSION
};

class ProbabilityModel {
public:
    virtual ~ProbabilityModel() noexcept = default;
    
    virtual std::pair<double, double> calculate_bic_with_prob(const std::vector<int>&) = 0;
    virtual void change_regular(double C) noexcept = 0;

    TypeModel get_name() const noexcept { return name; };

    int find_max(const std::vector<int>& sample){
        int max_val = -1;
        for(int i : sample) max_val = std::max(max_val, i);
        return max_val+1;
    }

protected:
    TypeModel name;
    
};

#endif // PROBABILITY_MODEL_H