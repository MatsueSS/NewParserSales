#include "ModelSelector.h"

#include <queue>

struct ModelGreater{
    bool operator()(ModelSelector::Result a, ModelSelector::Result b){
        return a.best_bic > b.best_bic;
    }
};

ModelSelector::ModelSelector(std::initializer_list<TypeModel> list)
{
    for(const auto& obj : list){
        models.emplace(FactoryModel::create(obj));
    }
}

void ModelSelector::add_model(TypeModel type) noexcept
{
    models.emplace(FactoryModel::create(type));
}

ModelSelector::Result ModelSelector::select_best(const std::vector<int>& sample)
{
    if(models.empty()) throw EmptySampleProbabilityModelException("ModelSelector must have at least one model\n");
    std::priority_queue<ModelSelector::Result, std::vector<ModelSelector::Result>, ModelGreater> pq;
    for(const auto& obj : models){
        Result r;
        try{
            r.best_probability = obj->predict_probability(sample);
            r.best_bic = obj->calculate_bic(sample);
            r.name = obj->get_name();
            pq.emplace(std::move(r));
        } catch (InapplicabilityProbabilityModelException& exc) {
            continue;
        } catch (ZeroDivisionForecastException& e){
            continue;
        }
    }
    if(pq.empty()) throw NoSuitableProbabilityException("all models no suitable");
    return pq.top();
}

bool ModelSelector::has_model(TypeModel type) const noexcept
{
    return models.find(type) != models.end();
}