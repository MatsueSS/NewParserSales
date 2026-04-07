#include "ROC_AUC.h"

double ROC_AUC::roc_auc(const std::vector<double>& p, const std::vector<double>& q) const noexcept
{
    int all = p.size()*q.size();
    double sum = 0;
    for(double i : p){
        for(double j : q){
            if(i > j) sum += 1;
            else if(i == j) sum += 0.5;
        }
    }
    return sum/all;
}