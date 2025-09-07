#include "PyLoader.h"

#include <cstdlib>

int PyLoader::load(std::string query) noexcept
{
    return WEXITSTATUS(system(query.c_str()));
}