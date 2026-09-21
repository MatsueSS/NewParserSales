#ifndef FACTORY_MODEL_H
#define FACTORY_MODEL_H

#include "Models/ProbabilityModel.h"

#include "Models/GeometricModel.h"
#include "Models/LogisticRegressionModel.h"

#include <memory>

class FactoryModel{
public:
    static std::unique_ptr<ProbabilityModel> create(TypeModel type){
        switch(type){
            case TypeModel::GEOMETRIC_MODEL:
                return std::make_unique<GeometricModel>();
            case TypeModel::LOGISTIC_REGRESSION:
                return std::make_unique<LogisticRegressionModel>();
            default:
                throw BadTypeProbabilityModelException("Invalid Type for probability model");
        }
    }
};

#endif // FACTORY_MODEL_H