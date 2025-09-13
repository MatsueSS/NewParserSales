#ifndef _GOOD_FUNCS_H_
#define _GOOD_FUNCS_H_

#include <string>
#include <chrono>

std::string get_id() noexcept;
std::string get_conn() noexcept;
std::chrono::year_month_day get_date_now() noexcept;
void update_offset(const std::string& offset) noexcept;
std::string get_last_offset() noexcept;

#endif //_GOOD_FUNCS_H_