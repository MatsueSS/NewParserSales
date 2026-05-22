#ifndef GEOMETRIC_MODEL_H
#define GEOMETRIC_MODEL_H

#include "Models/ProbabilityModel.h"
#include "Forecast.h"

class GeometricModel : public ProbabilityModel {
public:
    GeometricModel();

    //The sample must include strictly positive elements  - else throw or UB
    virtual double predict_probability(const std::vector<int>&) override;
    virtual double calculate_bic(const std::vector<int>&) override;

    virtual void change_regular(double C) noexcept override {}

private:
    Forecast forecast;

    std::vector<int> get_data(const std::vector<int>&) const noexcept;

};

#endif // GEOMETRIC_MODEL_H