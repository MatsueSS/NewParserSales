#ifndef TELEGRAM_USER
#define TELEGRAM_USER

//Here is the code that describes the bot user.

#include "PoolCards.h"

#include <unordered_set>
#include <string>
#include <memory>

class TelegramUserException : public std::exception{
protected:
    std::string msg;

public:
    TelegramUserException(std::string);
    TelegramUserException(const TelegramUserException&);

    const char* what() const noexcept override;
};

class TelegramUser{
private:
    std::string id;
    std::unordered_set<uint32_t> lovely_product;
    std::shared_ptr<PoolCards> ptr_pc;

public:
    TelegramUser(std::string, std::shared_ptr<PoolCards> ptr_pc);

    TelegramUser(const TelegramUser&) = default;
    TelegramUser& operator=(const TelegramUser&) = default;

    TelegramUser(TelegramUser&&) noexcept = default;
    TelegramUser& operator=(TelegramUser&&) noexcept = default;

    template<typename Type>
    void add_product(Type&&);

    template<typename Type>
    void del_product(Type&&);

    template<typename Type>
    bool is_has_product(Type&&) const;

    const std::unordered_set<uint32_t>& get_cards() const noexcept;

    std::string get_id() const noexcept;
};

template<typename Type>
void TelegramUser::add_product(Type&& str){
    if constexpr(std::is_same<std::decay_t<Type>, std::string>::value){
        uint32_t temp = ptr_pc->get_index(std::forward<Type>(str));
        lovely_product.emplace(temp);
    }
    else if constexpr(std::is_same<std::decay_t<Type>, uint32_t>::value){
        lovely_product.emplace(std::forward<Type>(str));
    }
    else
        throw TelegramUserException("Value-Type must be string or uint32_t\n");
}

template<typename Type>
void TelegramUser::del_product(Type&& str){
    if constexpr(std::is_same<std::decay_t<Type>, std::string>::value){
        uint32_t temp = ptr_pc->get_index(std::forward<Type>(str));
        lovely_product.erase(temp);
    }
    else if constexpr(std::is_same<std::decay_t<Type>, uint32_t>::value){
        lovely_product.erase(std::forward<Type>(str));
    }
    else
        throw TelegramUserException("Value-Type must be string or uint32_t\n");
}

template<typename Type>
bool TelegramUser::is_has_product(Type&& str) const {
    if constexpr(std::is_same<std::decay_t<Type>, std::string>::value){
        uint32_t temp = ptr_pc->get_index(std::forward<Type>(str));
        return lovely_product.count(temp);
    }
    else if constexpr(std::is_same<std::decay_t<Type>, uint32_t>::value){
        return lovely_product.count(std::forward<Type>(str));
    }
    else
        throw TelegramUserException("Value-Type must be a string or uint32_t\n");

    return false;
}

#endif // TELEGRAM_USER