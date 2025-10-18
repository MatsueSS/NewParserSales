#ifndef _TELEGRAM_USER_
#define _TELEGRAM_USER_

//Here is the code that describes the bot user.

#include "TelegramSender.h"

#include <unordered_set>
#include <string>

class TelegramUserException : public std::exception{
protected:
    std::string msg;

public:
    TelegramUserException(std::string);

    const char* what() const noexcept override;
};

class TelegramUser{
private:
    std::string id;
    std::unordered_set<std::string> lovely_product;

public:
    TelegramUser(std::string);

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

    std::unordered_set<std::string> get_cards() const;

    template<typename Type>
    void notify(Type&&) const;

    std::string get_id() const;
};

template<typename Type>
void TelegramUser::add_product(Type&& str){
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw TelegramUserException("Value-Type must be a string\n");

    lovely_product.emplace(std::forward<Type>(str));
}

template<typename Type>
void TelegramUser::del_product(Type&& str){
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw TelegramUserException("Value-Type must be a string\n");

    lovely_product.erase(std::forward<Type>(str));
}

template<typename Type>
bool TelegramUser::is_has_product(Type&& str) const {
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw TelegramUserException("Value-Type must be a string\n");

    return lovely_product.count(std::forward<Type>(str));
}

template<typename Type>
void TelegramUser::notify(Type&& str) const {
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw TelegramUserException("Value-Type must be a string\n");

    if(!lovely_product.count(str))
        return;

    auto ptr = TelegramSender::get_instance();
    ptr->call(id, type_msg::send, std::forward<Type>(str));
}

#endif //_TELEGRAM_USER_