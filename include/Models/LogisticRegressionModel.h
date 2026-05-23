#ifndef LOGISTIC_REGRESSION_MODEL_H
#define LOGISTIC_REGRESSION_MODEL_H

#include "Models/ProbabilityModel.h"

class LogisticRegressionModel : public ProbabilityModel{
public:
    LogisticRegressionModel() = default;
    
    virtual std::pair<double, double> calculate_bic_with_prob(const std::vector<int>&) override;

    virtual void change_regular(double C) noexcept override;

};

#endif