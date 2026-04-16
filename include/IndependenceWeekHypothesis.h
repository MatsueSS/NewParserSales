#ifndef _INDEPENDENCE_FROM_WEEK_HYPOTHESIS_H_
#define _INDEPENDENCE_FROM_WEEK_HYPOTHESIS_H_

#include "Hypothesis.h"

class IndependenceWeekHypothesis : public Hypothesis{
public:
    virtual int check_hypothesis(const std::string& title, const std::string& start_date,
        const std::string& end_date, double p_value) const override;

protected:
    virtual std::vector<std::vector<int>> prepare_table(const std::string& title, const std::string& start_date,
        const std::string& end_date) const override;

};

#endif