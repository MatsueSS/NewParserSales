#ifndef _GEOMETRIC_MODEL_H_
#define _GEOMETRIC_MODEL_H_

#include "ProbabilityModel.h"
#include "Forecast.h"

class GeometricModel : public ProbabilityModel {
public:
    virtual double predict_probability(const std::vector<int>&) const noexcept override;
    virtual double calculate_bic(const std::vector<int>&) const noexcept override;

private:
    Forecast forecast;

    std::vector<int> get_data(const std::vector<int>&) const noexcept;

};

#endif