#ifndef _RECOMMENDATIONS_H_
#define _RECOMMENDATIONS_H_

#include "PoolCards.h"
#include "TelegramUser.h"

#include <string>
#include <vector>
#include <exception>
#include <memory>

class RecommendationsException : public std::exception {
protected:
    std::string msg;

public:
    RecommendationsException(std::string msg) : msg(std::move(msg)) {}
    RecommendationsException(const RecommendationsException& obj) : msg(obj.msg) {}

    const char* what() const noexcept override { return msg.c_str(); };
};

class BadTypeFactoryRecommendationsException : public RecommendationsException {
public:
    BadTypeFactoryRecommendationsException(std::string msg) : RecommendationsException(std::move(msg)) {}

};

enum class RecType {
    MATRIX
};

class Recommendations{
protected:
    std::shared_ptr<PoolCards> ptr_pc;
    std::shared_ptr<std::unordered_map<std::string, TelegramUser>> matrix;

public:
    Recommendations(std::shared_ptr<PoolCards> ptr_pc, std::shared_ptr<std::unordered_map<std::string, TelegramUser>> matrix) : ptr_pc(ptr_pc), matrix(matrix) {}

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