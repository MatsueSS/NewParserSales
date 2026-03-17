#include "good_funcs.h"

#include "HiSquare.h"
#include "PostgresDB.h"

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

std::string converte_ymd(const std::chrono::year_month_day& ymd) noexcept {
    int year = int(ymd.year());
    unsigned month = unsigned(ymd.month());
    unsigned day = unsigned(ymd.day());
    std::ostringstream oss;
    oss << year << "-"
        << std::setw(2) << std::setfill('0') << month << "-"
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

std::chrono::year_month_day converte_string(const std::string& str) noexcept{
    unsigned int year, month, day;
    char delim1, delim2;
    std::istringstream iss(str);
    iss >> year >> delim1 >> month >> delim2 >> day;
    return std::chrono::year{static_cast<int>(year)} / std::chrono::month{month} / std::chrono::day{day};
}

void check_independence_week()
{
    HiSquare h;

    PostgresDB db;
    db.connect(get_conn());
    std::vector<std::vector<std::string>> result = db.fetch(std::string("SELECT DISTINCT title FROM cards;"), std::vector<std::string>{});
    std::ofstream file("../sensetive_res/independence_for_week.txt");

    int count = 0;
    for(const auto& vec : result){
        try{
            bool r = h.independence_from_week(vec[0], "2025-09-01", "2026-03-14", 0.95);
            if(!r) file << r << ' ' << ++count << ' ' << vec[0] << '\n';
        } catch(HiSquareException& e){
            continue;
        }
    }
}

void check_independence_season()
{
    HiSquare h;

    PostgresDB db;
    db.connect(get_conn());
    std::vector<std::vector<std::string>> result = db.fetch(std::string("SELECT DISTINCT title FROM cards;"), std::vector<std::string>{});
    std::ofstream file("../sensetive_res/independence_for_season.txt");

    int count = 0;
    for(const auto& vec : result){
        try{
            bool r = h.independence_from_season(vec[0], 0.95);
            if(!r) file << r << ' ' << ++count << ' ' << vec[0] << '\n';
        } catch(HiSquareException& e){
            continue;
        }
    }
}

std::chrono::year_month_day get_previous_or_current_saturday() noexcept
{    
    auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    std::chrono::weekday wd{today};
    
    int days_back = (wd.c_encoding() + 1) % 7;
    
    auto saturday = today - std::chrono::days(days_back);
    return std::chrono::year_month_day{saturday};
}

std::string saturday_to_string(const std::chrono::year_month_day& ymd) noexcept
{
    std::ostringstream oss;
    oss << std::setfill('0') << (int)ymd.year() << "-" << std::setw(2) << (unsigned)ymd.month() << "-" << (unsigned)ymd.day();
    return oss.str();
}