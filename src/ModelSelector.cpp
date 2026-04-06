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
        models.emplace_back(FacrotyModel::create(obj));
    }
}

ModelSelector::Result ModelSelector::select_best(const std::vector<int>& sample) noexcept
{
    std::priority_queue<ModelSelector::Result, std::vector<ModelSelector::Result>, ModelGreater> pq;
    for(const auto& obj : models){
        Result r;
        r.best_bic = obj->calculate_bic(sample);
        r.best_probability = obj->predict_probability(sample);
        r.name = obj->get_name();
        pq.emplace(std::move(r));
    }
    return pq.top();
}