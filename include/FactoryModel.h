#ifndef FACTORY_MODEL_H
#define FACTORY_MODEL_H

#include "Models/ProbabilityModel.h"

#include "Models/GeometricModel.h"
#include "Models/MarkovChain1Model.h"
#include "Models/MarkovChain2Model.h"

#include <memory>

class FactoryModel{
public:
    static std::unique_ptr<ProbabilityModel> create(TypeModel type){
        switch(type){
            case TypeModel::GEOMETRIC_MODEL:
                return std::make_unique<GeometricModel>();
            case TypeModel::MARKOV_CHAIN_1_MODEL:
                return std::make_unique<MarkovChain1Model>();
            case TypeModel::MARKOV_CHAIN_2_MODEL:
                return std::make_unique<MarkovChain2Model>();
            default:
                throw BadTypeProbabilityModelException("Invalid Type for probability model");
        }
    }
};

#endif // FACTORY_MODEL_H