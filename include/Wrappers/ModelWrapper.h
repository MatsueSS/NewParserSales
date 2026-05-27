#ifndef MODEL_WRAPPER_H
#define MODEL_WRAPPER_H

#include "Features_regression/Feature.h"

#include <linear.h>
#include <memory>
#include <functional>
#include <vector>
#include <exception>

class LogisticRegressionModel;

using ModelDeleter = std::function<void(model*)>;
using ProblemDeleter = std::function<void(problem*)>;

using ModelWrapperPTR = std::unique_ptr<model, ModelDeleter>;
using ProblemPTR = std::unique_ptr<problem, ProblemDeleter>;

class ModelWrapperException : public std::exception {
protected:
    std::string msg;

public:
    ModelWrapperException(std::string msg);
    ModelWrapperException(const ModelWrapperException& obj);

    const char * what() const noexcept override;

};

class NoInitModelWrapperException : public ModelWrapperException {
public:
    NoInitModelWrapperException(std::string msg);

};

class NotEnoughDataModelWrapperException : public ModelWrapperException {
public:
    NotEnoughDataModelWrapperException(std::string msg);
    
};

class ModelWrapper {
public:
    ModelWrapper();

    ModelWrapper(const ModelWrapper&) = delete;
    ModelWrapper& operator=(const ModelWrapper&) = delete;

    ModelWrapper(ModelWrapper&&) noexcept;
    ModelWrapper& operator=(ModelWrapper&&) noexcept;

    void change_C(double C) noexcept;

    void train_model();

    bool is_trained() const noexcept;

    std::vector<double> get_weight() const;
    double get_probability() const;
    double get_test_correct() const;
    double get_train_correct() const;

    void set_signs(std::vector<result_normalize>&& signs, const std::vector<double>& sample, int train_size);

    //for use bic
    friend LogisticRegressionModel;

private:
    int train_size, n, count_signs;
    ProblemPTR prob_ptr;
    ModelWrapperPTR mw_ptr;
    parameter param;
    std::vector<result_normalize> features;
    std::vector<double> sample;

};

#endif