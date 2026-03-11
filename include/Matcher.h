#ifndef _MATCHER_H_
#define _MATCHER_H_

#include <vector>
#include <string>
#include <unordered_map>

class Matcher{
public:
    virtual const std::vector<std::string>& find_matches(const std::string& str) const noexcept = 0;
    virtual ~Matcher() = default;

private:
    std::unordered_map<std::string, std::vector<std::string>> dict;
};

#endif