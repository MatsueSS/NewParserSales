#ifndef LOGISTIC_REGRESSION_MODEL_H
#define LOGISTIC_REGRESSION_MODEL_H

#include "Models/ProbabilityModel.h"
#include "Wrappers/ModelWrapper.h"

class LogisticRegressionModel : public ProbabilityModel{
public:
    LogisticRegressionModel();
    
    virtual std::pair<double, double> calculate_bic_with_prob(const std::vector<int>&) override;

    virtual void change_regular(double C) noexcept override;

private:
    ModelWrapper mw;

    std::vector<int> find_tuple_features(int n, const std::vector<result_normalize>& features) const;
    double calculate_bic(std::vector<std::vector<double>>&& X);

    double make_train(const std::vector<int>& sample);    

};

#endif