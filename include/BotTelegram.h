#ifndef _BOT_TELEGRAM_H_
#define _BOT_TELEGRAM_H_

#include "TelegramUser.h"

#include <unordered_map>

class BotTelegramException : public std::exception{
protected:
    std::string msg;

public: 
    BotTelegramException(std::string);

    const char* what() const noexcept override;
};

class BotTelegram{
private:
    std::unordered_map<std::string, TelegramUser> users;
    std::jthread worker;

    void check_message(std::stop_token) const;

public:
    BotTelegram();

    BotTelegram(const BotTelegram&) = delete;
    BotTelegram& operator=(const BotTelegram&) = delete;

    BotTelegram(BotTelegram&&) noexcept = default;
    BotTelegram& operator=(BotTelegram&&) noexcept = default;

    template<typename Type>
    void add_user(Type&&);

    template<typename Type>
    void del_user(Type&&);

    template<typename Type>
    bool is_has_user(Type&&) const;

    template<typename Type>
    void notify_all(Type&&) const;

    ~BotTelegram() = default;
};

template<typename Type>
void BotTelegram::add_user(Type&& user){
    if constexpr(!std::is_same<std::decay_t<Type>, TelegramUser>::value)
        throw BotTelegramException("Value-Type must be a TelegramUser\n");

    users.insert({user.get_id(), std::forward<Type>(user)});
}

template<typename Type>
void BotTelegram::del_user(Type&& user){
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw BotTelegramException("Value-Type must be a string\n");

    users.erase(user);
}

template<typename Type>
bool BotTelegram::is_has_user(Type&& user) const {
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw BotTelegramException("Value-type must be a string\n")

    return users.count(user);
}

template<typename Type>
void BotTelegram::notify_all(Type&& notifi) const {
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw BotTelegramException("Value-Type must be a string\n");

    for(const auto& obj : users)
        obj.second.notify(notifi);
}

#endif //_BOT_TELEGRAM_H_