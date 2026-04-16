#include "IndependenceWeekHypothesis.h"

#include "PostgresDB.h"
#include "good_funcs.h"
#include "HiSquare.h"

#include <map>
#include <boost/math/distributions/chi_squared.hpp>

int IndependenceWeekHypothesis::check_hypothesis(const std::string& title, const std::string& start_date,
    const std::string& end_date, double p_value) const
{
    auto table = prepare_table(title, start_date, end_date);

    HiSquare hs;
    double hi_square = hs.find_hi_square(table);

    int freedom = (COUNT_WEEKS-1)*(COUNT_RESULT-1);
    boost::math::chi_squared dist(freedom);

    return hi_square < boost::math::quantile(dist, p_value);
}

std::vector<std::vector<int>> IndependenceWeekHypothesis::prepare_table(const std::string& title,
    const std::string& start_date, const std::string& end_date) const
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
        throw ZeroSampleHypothesisException("Zero rows for " + title);
    for(const auto& row : discounts){
        table[weeks[row[DATE_IN_ROW]] - 1][0]++;
    }
    for(int i = 0; i < COUNT_WEEKS; ++i){
        table[i][1] = count_weeks[i]-table[i][0];
    }

    return table;
}