#include "ROC_AUC.h"

ROC_AUC_Exception::ROC_AUC_Exception(std::string msg) : msg(std::move(msg)) {}
ROC_AUC_Exception::ROC_AUC_Exception(const ROC_AUC_Exception& obj) : msg(obj.msg) {}

const char * ROC_AUC_Exception::what() const noexcept { return msg.c_str(); }

EmptySampleROC_AUC_Exception::EmptySampleROC_AUC_Exception(std::string msg) : ROC_AUC_Exception(std::move(msg)) {}

BadValueROC_AUC_Exception::BadValueROC_AUC_Exception(std::string msg) : ROC_AUC_Exception(std::move(msg)) {}

double ROC_AUC::roc_auc(const std::vector<double>& p, const std::vector<double>& q) const
{
    long long all = p.size()*q.size();
    if(all == 0) throw EmptySampleROC_AUC_Exception("sample p or q was empty");
    double sum = 0;
    for(double i : p){
        if(i < 0 || i > 1) throw BadValueROC_AUC_Exception("probability must be in [0,1]");
        for(double j : q){
            if(j < 0 || j > 1) throw BadValueROC_AUC_Exception("probability must be in [0,1]");
            if(i > j) sum += 1;
            else if(i == j) sum += 0.5;
        }
    }
    return sum/all;
}