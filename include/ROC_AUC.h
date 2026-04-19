#ifndef ROC_AUC_H
#define ROC_AUC_H

#include <vector>

class ROC_AUC {
public:
    double roc_auc(const std::vector<double>&, const std::vector<double>&) const noexcept;
    
};

#endif // ROC_AUC_H