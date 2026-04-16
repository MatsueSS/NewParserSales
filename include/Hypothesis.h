#ifndef _HYPOTHESIS_H_
#define _HYPOTHESIS_H_

#define REJECTED 0

#define COUNT_WEEKS 5
#define COUNT_RESULT 2
#define DATE_IN_ROW 4
#define COUNT_SEASON 2

#include <vector>
#include <string>
#include <exception>

class HypothesisException : public std::exception{
protected:
    std::string msg;

public:
    HypothesisException(std::string msg);
    HypothesisException(const HypothesisException& obj);

    const char * what() const noexcept override;

};

class ZeroSampleHypothesisException : public HypothesisException{
public:
    ZeroSampleHypothesisException(std::string msg);
    
};

class Hypothesis{
public:
    virtual int check_hypothesis(const std::string& title, const std::string& start_date,
        const std::string& end_date, double p_value) const = 0;

protected:
    virtual std::vector<std::vector<int>> prepare_table(const std::string& title, const std::string& start_date,
        const std::string& end_date) const = 0;

    std::vector<std::string> generate_all_saturdays(const std::string& start_date, const std::string& end_date) const noexcept;
    int get_week_of_month(int day) const noexcept;
    int get_season_index(int month) const noexcept;

};

#endif