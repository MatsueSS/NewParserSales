#ifndef MODEL_SELECTOR_H
#define MODEL_SELECTOR_H

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

    Result select_best(const std::vector<int>&);

    void add_model(TypeModel type) noexcept;

private:
    std::vector<std::unique_ptr<ProbabilityModel>> models;

};

#endif // MODEL_SELECTOR_H