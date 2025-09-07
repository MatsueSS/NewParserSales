#include "good_funcs.h"

#include <fstream>

std::string get_id() noexcept {
    std::ifstream file("../.env");
    std::string temp;
    std::getline(file, temp);
    std::getline(file, temp);

    return temp;
}