#include "HiSquare.h"

#include "Forecast.h"
#include "PostgresDB.h"
#include "good_funcs.h"

#define COUNT_WEEKS 5
#define COUNT_RESULT 2
#define DATE_IN_ROW 4
#define COUNT_SEASON 2

#include <boost/math/distributions/chi_squared.hpp>
#include <map>

HiSquareException::HiSquareException(std::string msg) : msg(std::move(msg)) {}

const char* HiSquareException::what() const noexcept { return msg.c_str(); }

double HiSquare::find_hi_square(const std::vector<std::vector<int>>& table) const noexcept
{
    int size_rows = table.size(), size_column = table[0].size();
    std::vector<int> sum_of_rows (size_rows, 0);
    std::vector<int> sum_of_columns (size_column, 0);
    int sum = 0;

    for(int i = 0; i < size_rows; ++i){
        for(int j = 0; j < size_column; ++j){
            sum_of_rows[i] += table[i][j];
        }
        sum += sum_of_rows[i];
    }

    for(int i = 0; i < size_column; ++i){
        for(int j = 0; j < size_rows; ++j){
            sum_of_columns[i] += table[j][i];
        }
    }

    double hi = 0.0;
    
    for(int i = 0; i < size_rows; ++i){
        for(int j = 0; j < size_column; ++j){
            double frequency = (sum_of_rows[i]*sum_of_columns[j])/(double)sum;
            if(frequency > 0){
                double temp = table[i][j]-frequency;
                hi += (temp*temp)/frequency;
            }
        }
    }

    return hi;
}

bool HiSquare::independence_from_week(const std::string& title, const std::string& start_date, const std::string& end_date, double p_value) const
{
    std::vector<std::string> all_saturdays = generate_all_saturdays(start_date, end_date);

    std::map<std::string, int> weeks;
    for(auto& sat : all_saturdays){
        int day = std::stoi(sat.substr(8, 2));
        weeks[sat] = get_week_of_month(day);
    }

    std::vector<int> count_weeks(COUNT_WEEKS,0);
    for(const auto& obj : weeks){
        count_weeks[(obj.second) - 1]++;
    }

    PostgresDB db;
    db.connect(get_conn());

    std::vector<std::vector<int>> table (COUNT_WEEKS, std::vector<int>(COUNT_RESULT, 0));

    std::vector<std::vector<std::string>> discounts;
    discounts = db.fetch(std::string("SELECT DISTINCT ON(date) * FROM cards WHERE title = $1 AND discount IS NOT NULL ORDER BY date DESC;"), std::vector<std::string>{title});
    if(discounts.empty())
        throw HiSquareException("Zero rows for " + title);
    for(const auto& row : discounts){
        table[weeks[row[DATE_IN_ROW]] - 1][0]++;
    }
    for(int i = 0; i < COUNT_WEEKS; ++i){
        table[i][1] = count_weeks[i]-table[i][0];
    }

    double hi_square = find_hi_square(table);

    int freedom = (COUNT_WEEKS-1)*(COUNT_RESULT-1);
    boost::math::chi_squared dist(freedom);

    return hi_square < boost::math::quantile(dist, p_value);
}

bool HiSquare::independence_from_season(const std::string& title, double p_value) const
{
    std::vector<std::vector<int>> table (COUNT_SEASON, std::vector<int>(COUNT_RESULT, 0));

    std::vector<std::string> all_saturdays = generate_all_saturdays("2025-09-01", "2026-02-28");
    for(const auto& date : all_saturdays) {
        int month = std::stoi(date.substr(5, 2));
        int season_index = get_season_index(month);
        if(season_index >= 0 && season_index < COUNT_SEASON) {
            table[season_index][1]++;
        }
    }
    
    PostgresDB db;
    db.connect(get_conn());
    
    std::vector<std::vector<std::string>> discounts;
    discounts = db.fetch(std::string("SELECT DISTINCT ON(date) * FROM cards WHERE title = $1 AND discount IS NOT NULL ORDER BY date DESC;"), std::vector<std::string>{title});
    if(discounts.empty())
        throw HiSquareException("Zero rows for " + title);

    for(const auto& row : discounts) {
        int month = std::stoi(row[DATE_IN_ROW].substr(5, 2));
        int season_index = get_season_index(month);
        if(season_index >= 0 && season_index < COUNT_SEASON) {
            table[season_index][0]++;
            table[season_index][1]--;
        }
    }

    double hi_square = find_hi_square(table);

    int freedom = (COUNT_RESULT-1)*(COUNT_SEASON-1);
    boost::math::chi_squared diff(freedom);

    return hi_square < boost::math::quantile(diff, p_value);
}

int HiSquare::get_season_index(int month) const noexcept {
    if(month >= 3 && month <= 5) return 2;
    if(month >= 6 && month <= 8) return 3;
    if(month >= 9 && month <= 11) return 0;
    if(month == 12 || month <= 2) return 1;
    return -1;
}

std::vector<std::string> HiSquare::generate_all_saturdays(const std::string& start_date, const std::string& end_date) const noexcept
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

int HiSquare::get_week_of_month(int day) const noexcept
{
    return (day - 1)/7 + 1;
}