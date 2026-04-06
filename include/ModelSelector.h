#ifndef _MODEL_SELECTOR_H_
#define _MODEL_SELECTOR_H_

#include "FactoryModel.h"

#include <vector>
#include <memory>

class ModelSelector{
public:
    ModelSelector(std::initializer_list<TypeModel>);

    struct Result{
        double best_bic;
        double best_probability;
        TypeModel name;
    };

    Result select_best(const std::vector<int>&) noexcept;

private:
    std::vector<std::unique_ptr<ProbabilityModel>> models;

};

#endif