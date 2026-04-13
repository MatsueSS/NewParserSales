#ifndef _MODEL_SELECTOR_H_
#define _MODEL_SELECTOR_H_

#include "FactoryModel.h"

#include <vector>
#include <memory>
#include <exception>

class ModelSelectorException : public std::exception{
protected:
    std::string msg;

public:
    ModelSelectorException(std::string msg) noexcept;
    ModelSelectorException(const ModelSelectorException& obj) noexcept;

    const char * what() const noexcept override;

};

class EmptyContainerModelSelectorException : public ModelSelectorException{
public:
    EmptyContainerModelSelectorException(std::string msg) noexcept;

};

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

#endif