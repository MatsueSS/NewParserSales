#include "IndependenceSeasonHypothesis.h"

#include "PostgresDB.h"
#include "good_funcs.h"
#include "HiSquare.h"

#include <boost/math/distributions/chi_squared.hpp>

int IndependenceSeasonHypothesis::check_hypothesis(const std::string& title, const std::string& start_date,
    const std::string& end_date, double p_value) const
{
    HiSquare hs;
    double hi_square = hs.find_hi_square(prepare_table(title, start_date, end_date));

    int freedom = (COUNT_RESULT-1)*(COUNT_SEASON-1);
    boost::math::chi_squared diff(freedom);

    return hi_square < boost::math::quantile(diff, p_value);
}

std::vector<std::vector<int>> IndependenceSeasonHypothesis::prepare_table(const std::string& title, const std::string& start_date,
    const std::string& end_date) const
{
    std::vector<std::vector<int>> table (COUNT_SEASON, std::vector<int>(COUNT_RESULT, 0));

    std::vector<std::string> all_saturdays = generate_all_saturdays(start_date, end_date);
    for(const auto& date : all_saturdays) {
        int month = std::stoi(date.substr(5, 2));
        int season_index = static_cast<int>(get_season_index(month));
        if(season_index >= 0 && season_index < COUNT_SEASON) {
            table[season_index][1]++;
        }
    }
    
    PostgresDB db;
    db.connect(get_conn());
    
    std::vector<std::vector<std::string>> discounts;
    discounts = db.fetch(std::string("SELECT DISTINCT ON(date) * FROM cards WHERE title = $1 AND discount IS NOT NULL ORDER BY date DESC;"), std::vector<std::string>{title});
    if(discounts.empty()) throw ZeroSampleHypothesisException("Zero rows for " + title);

    for(const auto& row : discounts) {
        int month = std::stoi(row[DATE_IN_ROW].substr(5, 2));
        int season_index = static_cast<int>(get_season_index(month));
        if(season_index >= 0 && season_index < COUNT_SEASON) {
            table[season_index][0]++;
            table[season_index][1]--;
        }
    }

    return table;
}