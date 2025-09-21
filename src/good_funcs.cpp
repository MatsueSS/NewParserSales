#include "good_funcs.h"

#include <fstream>
#include <sstream>
#include <iomanip>

std::string get_id() noexcept {
    std::ifstream file("../.env");
    std::string temp;
    std::getline(file, temp);
    std::getline(file, temp);

    return temp;
}

std::string get_conn() noexcept {
    std::ifstream file("/home/michael/practisecpp/projects/ParserSales/.env");
    std::string temp, conn;
    getline(file, temp);
    getline(file, temp);
    getline(file, conn);
    return conn;
}

std::chrono::year_month_day get_date_now() noexcept {
    auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    auto current_date = std::chrono::year_month_day{today};
    return current_date;
}

void update_offset(const std::string& offset) noexcept {
    std::ifstream file("../.env");
    std::string token, id, url1, url2;
    std::getline(file, token);
    std::getline(file, id);
    std::getline(file, url1);
    std::getline(file, url2);
    file.close();
    std::ofstream nfile("../.env");
    nfile << token << '\n' << id << '\n' << url1 << '\n' << url2 << '\n' << offset << '\n';
    nfile.close();
}

std::string get_last_offset() noexcept{
    std::string temp, offset;
    std::ifstream file("../.env");
    std::getline(file, temp);
    std::getline(file, temp);
    std::getline(file, temp);
    std::getline(file, temp);
    std::getline(file, offset);
    return offset;
}

std::string get_date_str_now() noexcept{
    auto today_sys = std::chrono::system_clock::now();
    auto today_days = std::chrono::floor<std::chrono::days>(today_sys);
    std::chrono::year_month_day ymd{today_days};

    int year  = int(ymd.year());
    unsigned month = unsigned(ymd.month());
    unsigned day   = unsigned(ymd.day());

    std::ostringstream oss;
    oss << year << "-" 
        << std::setw(2) << std::setfill('0') << month << "-" 
        << std::setw(2) << std::setfill('0') << day;

    return oss.str(); // формат YYYY-MM-DD
}