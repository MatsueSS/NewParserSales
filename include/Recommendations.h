#ifndef _RECOMMENDATIONS_H_
#define _RECOMMENDATIONS_H_

#include <string>
#include <vector>

class Recommendations{
public:
    virtual ~Recommendations() = default;

    virtual void add_user(const std::string& id) noexcept {}
    virtual void del_user(const std::string& id) noexcept {}
    virtual void add_card(const std::string& id, const std::string& card) noexcept {}
    virtual void del_card(const std::string& id, const std::string& card) noexcept {}
    
    virtual std::vector<std::string> recommendation(const std::string& id) const = 0;
};

#endif