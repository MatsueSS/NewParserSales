#ifndef _GOOD_FUNCS_H_
#define _GOOD_FUNCS_H_

//Here are some handy features that I use often.

#include <string>
#include <chrono>

std::string get_id() noexcept;
std::string get_conn() noexcept;
std::chrono::year_month_day get_date_now() noexcept;
std::string get_date_str_now() noexcept;
void update_offset(const std::string& offset) noexcept;
std::string get_last_offset() noexcept;
std::string converte_ymd(const std::chrono::year_month_day&) noexcept;
std::chrono::year_month_day converte_string(const std::string& str) noexcept;

std::chrono::year_month_day get_previous_or_current_saturday() noexcept;
std::string saturday_to_string(const std::chrono::year_month_day& ymd) noexcept;

void check_independence_week();
void check_independence_season();

#endif //_GOOD_FUNCS_H_