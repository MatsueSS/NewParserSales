#ifndef MODEL_WRAPPER_H
#define MODEL_WRAPPER_H

#include "Models/LogisticRegressionModel.h"
#include "Features_regression/Feature.h"

#include <linear.h>
#include <memory>
#include <functional>
#include <vector>

using ModelDeleter = std::function<void(model*)>;
using ProblemDeleter = std::function<void(problem*)>;

using ModelWrapperPTR = std::unique_ptr<model, ModelDeleter>;
using ProblemPTR = std::unique_ptr<problem, ProblemDeleter>;

class ModelWrapper {
public:
    ModelWrapper();

    ModelWrapper(const ModelWrapper&) = delete;
    ModelWrapper& operator=(const ModelWrapper&) = delete;

    ModelWrapper(ModelWrapper&&) noexcept;
    ModelWrapper& operator=(ModelWrapper&&) noexcept;

    void change_C(double C) noexcept;

    void train_model() noexcept;

    bool is_trained() const noexcept;

    std::vector<double> get_weight() noexcept;
    double get_probability() noexcept;
    double get_test_correct() noexcept;
    double get_train_correct() noexcept;

    void set_signs(std::vector<result_normalize>&& signs, const std::vector<int>& sample, int train_size) noexcept;

    //for use bic
    friend LogisticRegressionModel;

private:
    int train_size, n, count_signs;
    ProblemPTR prob_ptr;
    ModelWrapperPTR mw_ptr;
    parameter param;
    std::vector<result_normalize> features;
    std::vector<int> sample;

};

#endif