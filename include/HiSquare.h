#ifndef _HI_SQUARE_H_
#define _HI_SQUARE_H_

#include <vector>
#include <string>
#include <exception>

class HiSquareException : public std::exception{
protected:
    std::string msg;

public:
    HiSquareException(std::string);

    const char* what() const noexcept override;
};

class HiSquare{
public:
    //Tests the hypothesis of independence of the discount to the week number
    bool independence_from_season(const std::string& title, double p_value) const;

    double find_hi_square(const std::vector<std::vector<int>>& table) const noexcept;

private:
    std::vector<std::string> generate_all_saturdays(const std::string& start, const std::string& end) const noexcept;

    int get_week_of_month(int day) const noexcept;
    int get_season_index(int month) const noexcept;
};

#endif