#ifndef MATCHER_H
#define MATCHER_H

/*
    Here is an abstract class for reading files with synonyms for products.
*/

#include "PoolCards.h"

#include <memory>

class MatcherException : public std::exception {
protected:
    std::string msg;

public:
    MatcherException(std::string msg) : msg(std::move(msg)) {}
    MatcherException(const MatcherException& obj) : msg(obj.msg) {}

    const char* what() const noexcept override { return msg.c_str(); }
};

class BadTypeMatcherException : public MatcherException {
public:
    BadTypeMatcherException(std::string msg) : MatcherException(std::move(msg)) {}

};

enum class ProdType{
    FILE_SEARCHER
};

class Matcher{
public:
    // filename can be txt file, json file or name of table
    Matcher(const std::string& filename, std::shared_ptr<PoolCards> ptr_pc) : filename(filename), ptr_pc(ptr_pc) {}

    template<typename Data>
    const std::vector<uint32_t>* find_matches(Data&& query) const noexcept; // POTENTIAL PROBLEM

    virtual ~Matcher() = default;

protected:
    std::unordered_map<std::string, std::vector<uint32_t>> dict;
    std::string filename;
    std::shared_ptr<PoolCards> ptr_pc;

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

#endif // MATCHER_H