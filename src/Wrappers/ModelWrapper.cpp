#include "Wrappers/ModelWrapper.h"

ModelWrapperException::ModelWrapperException(std::string msg) : msg(std::move(msg)) {}

ModelWrapperException::ModelWrapperException(const ModelWrapperException& obj) : msg(obj.msg) {}

const char * ModelWrapperException::what() const noexcept { return msg.c_str(); }

NoInitModelWrapperException::NoInitModelWrapperException(std::string msg) : ModelWrapperException(std::move(msg)) {}

NotEnoughDataModelWrapperException::NotEnoughDataModelWrapperException(std::string msg) : ModelWrapperException(std::move(msg)) {}

ModelWrapper::ModelWrapper() : prob_ptr(nullptr), mw_ptr(nullptr)
{
    param.solver_type = L2R_LR;
    param.C = 0.1;
    param.eps = 0.01;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    param.p = 0.1;
    param.init_sol = NULL;
}

ModelWrapper::ModelWrapper(ModelWrapper&& obj) noexcept : 
    train_size(obj.train_size), n(obj.n), count_signs(obj.count_signs),
    prob_ptr(std::move(obj.prob_ptr)), mw_ptr(std::move(obj.mw_ptr)),
    param(std::move(obj.param)), features(std::move(features)), sample(std::move(sample)) {}
    

ModelWrapper& ModelWrapper::operator=(ModelWrapper&& obj) noexcept
{
    if(this == &obj) return *this;

    this->train_size = obj.train_size;
    this->n = obj.n;
    this->count_signs = obj.count_signs;
    this->prob_ptr = std::move(obj.prob_ptr);
    this->mw_ptr = std::move(obj.mw_ptr);
    this->param = std::move(obj.param);
    this->features = std::move(obj.features);
    this->sample = std::move(obj.sample);

    return *this;
}

void ModelWrapper::change_C(double C) noexcept
{
    param.C = C;
}

void ModelWrapper::train_model()
{
    if(!prob_ptr) throw NoInitModelWrapperException("struct of problem wasn't initialized");

    model* model_ = train(prob_ptr.get(), &param);
    if(mw_ptr) mw_ptr.reset();
    mw_ptr = ModelWrapperPTR(model_, [](model* model_){ free_and_destroy_model(&model_); });
}

std::vector<double> ModelWrapper::get_weight() const
{
    if(!mw_ptr) throw NoInitModelWrapperException("method train_model() wasn't called");

    std::vector<double> weight(count_signs);
    for(int i = 0; i < count_signs; ++i){
        weight[i] = mw_ptr->w[i];
    }
    return weight;
}

double ModelWrapper::get_probability() const
{
    if(!mw_ptr) throw NoInitModelWrapperException("method train_model() wasn't called");

    std::vector<feature_node> next_point(count_signs+1);

    for(int i = 0; i < count_signs; ++i){
        next_point[i].index = i+1;
        next_point[i].value = features[i].last_norm;
    }

    next_point[count_signs].index = -1;

    double next_probs[2];
    predict_probability(mw_ptr.get(), next_point.data(), next_probs);  

    return next_probs[1];
}

void ModelWrapper::set_signs(std::vector<result_normalize>&& signs, const std::vector<double>& sample, int train_size)
{
    if(sample.size() < train_size) throw NotEnoughDataModelWrapperException("In sample not enough data");
    
    for(int i = 0; i < signs.size(); ++i){
        if(signs[i].norm_sample.size() < train_size) throw NotEnoughDataModelWrapperException("In signs (row: " + std::to_string(i) + ") not enough data");
    }

    if(prob_ptr) prob_ptr.reset();

    this->train_size = train_size;
    this->n = sample.size();
    this->count_signs = signs.size();

    this->features = std::move(signs);
    this->sample = sample;

    problem* prob = new problem;
    prob->l = train_size;
    prob->n = count_signs;
    prob->y = new double[train_size];
    prob->x = new feature_node*[train_size];

    for(int i = 0; i < train_size; ++i){
        prob->x[i] = new feature_node[count_signs+1];
        for(int j = 0; j < count_signs; ++j){
            prob->x[i][j].index = j+1;
            prob->x[i][j].value = features[j].norm_sample[i];
        }
        prob->x[i][count_signs].index = -1;
        prob->y[i] = sample[i];
    }

    prob_ptr = ProblemPTR(prob, [](problem* prob){
        delete[] prob->y;
        for(int i = 0; i < prob->l; ++i) delete[] prob->x[i];
        delete[] prob->x;
        delete prob;
    });
}

double ModelWrapper::get_train_correct() const
{
    if(!mw_ptr) throw NoInitModelWrapperException("method train_model() wasn't called");

    int train_correct = 0;
    for (int i = 0; i < train_size; i++) {
        double pred_class = predict(mw_ptr.get(), prob_ptr->x[i]);
        if (pred_class == sample[i]) train_correct++;
    }
    return static_cast<double>(train_correct) / train_size;
}

double ModelWrapper::get_test_correct() const
{
    if(!mw_ptr) throw NoInitModelWrapperException("method train_model() wasn't called");

    int test_correct = 0;
    for (int i = train_size; i < n; i++) {
        std::vector<feature_node> test_point(count_signs+1);

        for(int j = 0; j < count_signs; ++j){
            test_point[j].index = j+1;
            test_point[j].value = features[j].norm_sample[i];
        }

        test_point[count_signs].index = -1;
        
        double pred_class = predict(mw_ptr.get(), test_point.data());
        if (pred_class == sample[i]) test_correct++;
    }
    return n-train_size != 0 ? static_cast<double>(test_correct) / (n-train_size) : -1;
}

bool ModelWrapper::is_trained() const noexcept
{
    return mw_ptr != nullptr;
}