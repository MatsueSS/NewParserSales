#ifndef _ROC_AUC_H_
#define _ROC_AUC_H_

#include <vector>

class ROC_AUC {
public:
    double roc_auc(const std::vector<double>&, const std::vector<double>&) const noexcept;
    
};

#endif