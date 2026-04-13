#include "ModelSelector.h"

#include <queue>

ModelSelectorException::ModelSelectorException(std::string msg) noexcept : msg(std::move(msg)) {}
ModelSelectorException::ModelSelectorException(const ModelSelectorException& obj) noexcept : msg(obj.msg) {}

const char * ModelSelectorException::what() const noexcept { return msg.c_str(); }

EmptyContainerModelSelectorException::EmptyContainerModelSelectorException(std::string msg) noexcept : ModelSelectorException(std::move(msg)) {}

struct ModelGreater{
    bool operator()(ModelSelector::Result a, ModelSelector::Result b){
        return a.best_bic > b.best_bic;
    }
};

ModelSelector::ModelSelector(std::initializer_list<TypeModel> list)
{
    for(const auto& obj : list){
        models.emplace_back(FactoryModel::create(obj));
    }
}

void ModelSelector::add_model(TypeModel type) noexcept
{
    models.emplace_back(FactoryModel::create(type));
}

ModelSelector::Result ModelSelector::select_best(const std::vector<int>& sample)
{
    if(models.empty()) throw EmptyContainerModelSelectorException("ModelSelector must have at least one model\n");
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