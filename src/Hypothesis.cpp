#include "Hypothesis.h"

#include <chrono>

HypothesisException::HypothesisException(std::string msg) : msg(std::move(msg)) {}
HypothesisException::HypothesisException(const HypothesisException& obj) : msg(obj.msg) {}

const char * HypothesisException::what() const noexcept { return msg.c_str(); }

ZeroSampleHypothesisException::ZeroSampleHypothesisException(std::string msg) : HypothesisException(std::move(msg)) {}

FalseSeasonHypothesesException::FalseSeasonHypothesesException(std::string msg) : HypothesisException(std::move(msg)) {}

int Hypothesis::get_week_of_month(int day) const noexcept
{
    return (day - 1)/7 + 1;
}

std::vector<std::string> Hypothesis::generate_all_saturdays(const std::string& start_date, const std::string& end_date) const noexcept
{
    std::vector<std::string> saturdays;

    int start_year = std::stoi(start_date.substr(0, 4));
    int start_month = std::stoi(start_date.substr(5, 2));
    int start_day = std::stoi(start_date.substr(8, 2));

    int end_year = std::stoi(end_date.substr(0, 4));
    int end_month = std::stoi(end_date.substr(5, 2));
    int end_day = std::stoi(end_date.substr(8, 2));

    std::tm tm = {};
    tm.tm_year = start_year-1900;
    tm.tm_mon = start_month-1;
    tm.tm_mday = start_day;
    tm.tm_hour = 12;
    std::mktime(&tm);

    int current_dow = tm.tm_wday;
    int days_to_saturday = (6 - current_dow + 7) % 7;
    tm.tm_mday += days_to_saturday;
    std::mktime(&tm);

    while(true){
        if(tm.tm_year + 1900 > end_year) break;
        if(tm.tm_year + 1900 == end_year && tm.tm_mon + 1 > end_month) break;
        if(tm.tm_year + 1900 == end_year && tm.tm_mon + 1 == end_month && tm.tm_mday > end_day) break;

        char buffer[11];
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        saturdays.push_back(buffer);
        
        tm.tm_mday += 7;
        std::mktime(&tm);
    }

    return saturdays;
}

seasons Hypothesis::get_season_index(int month) const {
    if(month >= 3 && month <= 5) return seasons::SPRING;
    if(month >= 6 && month <= 8) return seasons::SUMMER;
    if(month >= 9 && month <= 11) return seasons::AUTUMN;
    if(month == 12 || month <= 2) return seasons::WINTER;
    throw FalseSeasonHypothesesException("function take bad month");
}