#ifndef _PY_LOADER_H_
#define _PY_LOADER_H_

#include <string>

class PyLoader{
public:
    static int load(std::string) noexcept;

};

#endif //_PY_LOADER_H_