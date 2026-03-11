#ifndef _MATCHER_H_
#define _MATCHER_H_

#include <vector>
#include <string>
#include <unordered_map>

class Matcher{
public:
    Matcher();

    template<typename Data>
    const std::vector<std::string>* find_matches(Data&& query) const noexcept;

    virtual ~Matcher() = default;

private:
    std::unordered_map<std::string, std::vector<std::string>> dict;

protected:
    virtual void load_data() = 0;

};

/**
 * @return pointer on result or nullptr
 * @note User must be checked result
 */
template<typename Data>
const std::vector<std::string>* Matcher::find_matches(Data&& query) const noexcept
{
    auto it = dict.find(std::forward<Data>(query));
    return it == dict.end() ? nullptr : &it->second;
}

#endif