#ifndef _RECOMMENDATIONS_H_
#define _RECOMMENDATIONS_H_

#include <string>
#include <vector>

class Recommendations{
public:
    virtual ~Recommendations() = default;

    virtual void add_user(const std::string& id) {}
    virtual void del_user(const std::string& id) {}
    virtual void add_card(const std::string& id, const std::string& card) {}
    virtual void del_card(const std::string& id, const std::string& card) {}

    virtual void add_user(std::string&& id) {}
    virtual void del_user(std::string&& id) {}
    virtual void add_card(std::string&& id, std::string&& card) {}
    virtual void del_card(std::string&& id, std::string&& card) {}
    
    virtual std::vector<std::string> recommendation(const std::string& id) const = 0;
};

#endif