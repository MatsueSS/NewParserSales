#ifndef _MATCHER_H_
#define _MATCHER_H_

#include <vector>
#include <string>
#include <unordered_map>

class Matcher{
public:
    Matcher() = default;

    template<typename Data>
    const std::vector<uint32_t>* find_matches(Data&& query) const noexcept;

    virtual ~Matcher() = default;

protected:
    std::unordered_map<std::string, std::vector<uint32_t>> dict;

    virtual void load_data() = 0;

};

/**
 * @return pointer on result or nullptr
 * @note User must be checked result
 */
template<typename Data>
const std::vector<uint32_t>* Matcher::find_matches(Data&& query) const noexcept
{
    auto it = dict.find(std::forward<Data>(query));
    return it == dict.end() ? nullptr : &it->second;
}

#endif