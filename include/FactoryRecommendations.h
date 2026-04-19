#ifndef FACTORY_RECOMMENDATIONS_H
#define FACTORY_RECOMMENDATIONS_H

#include "Recommendations.h"
#include "Matrix.h"

#include <memory>

class FactoryRecommendations {
public:
    static std::unique_ptr<Recommendations> create(RecType type, std::shared_ptr<PoolCards> ptr_pc, std::shared_ptr<std::unordered_map<std::string, TelegramUser>> m){
        switch(type){
            case RecType::MATRIX:
                return std::make_unique<Matrix>(ptr_pc, m);
            default:
                throw BadTypeFactoryRecommendationsException("Invalid type RecType\n");
        }
    }
};

#endif // FACTORY_RECOMMENDATIONS_H