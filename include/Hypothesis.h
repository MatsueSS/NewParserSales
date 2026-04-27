#ifndef HYPOTHESIS_H
#define HYPOTHESIS_H

#define REJECTED 0

#define COUNT_WEEKS 5 // maximum count weeks in a month
#define COUNT_RESULT 2 // I only consider binary situations
#define DATE_IN_ROW 4 // the position where the date is located in the database table
#define COUNT_SEASON 2 // the number of seasons I am considering

#include <vector>
#include <string>
#include <exception>

enum class seasons { AUTUMN = 0, WINTER = 1, SPRING = 2, SUMMER = 3 };

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

class FalseSeasonHypothesesException : public HypothesisException{
public:
    FalseSeasonHypothesesException(std::string msg);
    
};

class Hypothesis{
public:
    virtual int check_hypothesis(const std::string& title, const std::string& start_date,
        const std::string& end_date, double p_value) const = 0;

    seasons get_season_index(int month) const;

    virtual ~Hypothesis() = default;

protected:
    virtual std::vector<std::vector<int>> prepare_table(const std::string& title, const std::string& start_date,
        const std::string& end_date) const = 0;

    std::vector<std::string> generate_all_saturdays(const std::string& start_date, const std::string& end_date) const noexcept;
    int get_week_of_month(int day) const noexcept;

};

#endif // HYPOTHESIS_H