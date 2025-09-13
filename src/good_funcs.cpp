#include "good_funcs.h"

#include <fstream>

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