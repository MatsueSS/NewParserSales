#ifndef ROC_AUC_H
#define ROC_AUC_H

#include <vector>
#include <exception>
#include <string>

class ROC_AUC_Exception : public std::exception{
protected:
    std::string msg;

public:
    ROC_AUC_Exception(std::string msg);
    ROC_AUC_Exception(const ROC_AUC_Exception& obj);

    const char * what() const noexcept override;
};

class EmptySampleROC_AUC_Exception : public ROC_AUC_Exception{
public:
    EmptySampleROC_AUC_Exception(std::string msg);

};

class BadValueROC_AUC_Exception : public ROC_AUC_Exception{
public:
    BadValueROC_AUC_Exception(std::string msg);

};

class ROC_AUC {
public:
    double roc_auc(const std::vector<double>&, const std::vector<double>&) const;
    
};

#endif // ROC_AUC_H