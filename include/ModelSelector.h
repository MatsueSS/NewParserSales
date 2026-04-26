#ifndef MODEL_SELECTOR_H
#define MODEL_SELECTOR_H

#include "FactoryModel.h"

#include <set>
#include <memory>

struct ModelSelectorComp{
    using is_transparent = void;

    bool operator()(const std::unique_ptr<ProbabilityModel>& m1, const std::unique_ptr<ProbabilityModel>& m2) const noexcept { 
        return static_cast<int>(m1->get_name()) < static_cast<int>(m2->get_name()); 
    }

    bool operator()(const std::unique_ptr<ProbabilityModel>& m1, TypeModel type) const noexcept {
        return static_cast<int>(m1->get_name()) < static_cast<int>(type);
    }

    bool operator()(TypeModel type, const std::unique_ptr<ProbabilityModel>& m2) const noexcept {
        return static_cast<int>(type) < static_cast<int>(m2->get_name());
    }
};

class ModelSelector{
public:
    explicit ModelSelector(std::initializer_list<TypeModel>);

    ModelSelector(const ModelSelector&) = default;
    ModelSelector& operator=(const ModelSelector&) = default;

    ModelSelector(ModelSelector&&) noexcept = default;
    ModelSelector& operator=(ModelSelector&&) noexcept = default;

    struct Result{
        double best_bic;
        double best_probability;
        TypeModel name;
    };

    Result select_best(const std::vector<int>&);

    void add_model(TypeModel type) noexcept;

    bool has_model(TypeModel type) const noexcept;

private:
    std::set<std::unique_ptr<ProbabilityModel>, ModelSelectorComp> models;

};

#endif // MODEL_SELECTOR_H