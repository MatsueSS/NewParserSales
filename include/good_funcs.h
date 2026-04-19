#ifndef GOOD_FUNCS_H
#define GOOD_FUNCS_H

//Here are some handy features that I use often.

#include <string>
#include <chrono>

// read .env and give my user id in telegram
std::string get_id() noexcept;

// read .env and take string, which help u to connect to db
std::string get_conn() noexcept;

// get date now in std::chrono::year_month_day
std::chrono::year_month_day get_date_now() noexcept;

// get date now is std::string
std::string get_date_str_now() noexcept;

// shift offset
void update_offset(const std::string& offset) noexcept;

// get last offset
std::string get_last_offset() noexcept;

// help u converte date in std::chrono::year_month_day in std::string
std::string converte_ymd(const std::chrono::year_month_day&) noexcept;

// help u convert date in std::string in std::chrono::year_month_day
std::chrono::year_month_day converte_string(const std::string& str) noexcept;

// get previous or current saturday
std::chrono::year_month_day get_previous_or_current_saturday() noexcept;

std::string saturday_to_string(const std::chrono::year_month_day& ymd) noexcept;

// Hypothesis testing: does the discount depend on the week number?
void check_independence_week();

// Hypothesis testing - should I use 1 probability or divide by seasons for geometric distribution?
void check_independence_season();

#endif // GOOD_FUNCS_H