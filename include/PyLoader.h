#ifndef _PY_LOADER_H_
#define _PY_LOADER_H_

//The code allows you to call py scripts

#include <string>

class PyLoader{
public:
    static int load(std::string) noexcept;

};

#endif //_PY_LOADER_H_