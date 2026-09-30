#ifndef FEATURE_BOOST_WEEK_H
#define FEATURE_BOOST_WEEK_H

#include "FeatureBoost.h"

#include <chrono>

class FeatureBoostWeek : public FeatureBoost<FeatureBoostWeek>{
private:
    friend class FeatureBoost<FeatureBoostWeek>;

    static type_feature_boost name_impl() noexcept {
        return type_feature_boost::week;
    }

    template<ConceptBoostHistory T>
    static double compute_impl(T&& sample) noexcept {
        if(sample.empty()) return INT_MIN;
        std::chrono::year_month_day ymd = sample.back().get_date();
        ymd = ymd.year() / std::chrono::month{1} / std::chrono::day{1};
        std::chrono::sys_days start = ymd, cur = sample.back().get_date();
        std::int32_t days = (cur-start).count();
        return days/7+1;
    }
};

#endif
