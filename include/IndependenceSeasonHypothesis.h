#ifndef INDEPENDENCE_FROM_SEASON_HYPOTHESIS_H
#define INDEPENDENCE_FROM_SEASON_HYPOTHESIS_H

#include "Hypothesis.h"

class IndependenceSeasonHypothesis : public Hypothesis{
public:
    virtual int check_hypothesis(const std::string& title, const std::string& start_date,
        const std::string& end_date, double p_value) const override;

protected:
    virtual std::vector<std::vector<int>> prepare_table(const std::string& title, const std::string& start_date,
        const std::string& end_date) const override;

};

#endif // INDEPENDENCE_FROM_SEASON_HYPOTHESIS_H