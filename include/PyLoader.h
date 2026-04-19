#ifndef PY_LOADER_H
#define PY_LOADER_H

//The code allows you to call py scripts

#include <string>

class PyLoader{
public:
    static int load(std::string) noexcept;

};

#endif // PY_LOADER_H