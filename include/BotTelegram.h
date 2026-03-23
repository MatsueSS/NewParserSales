#ifndef _BOT_TELEGRAM_H_
#define _BOT_TELEGRAM_H_

//Here is the code that describes the bot in Telegram.

#include "TelegramUser.h"
#include "ProductSearcher.h"
#include "ProductRecommendations.h"
#include "PrefixTree.h"
#include "UserStateMaching.h"
#include "PoolCards.h"

#include <unordered_map>
#include <thread>
#include <vector>
#include <set>

class BotTelegramException : public std::exception{
protected:
    std::string msg;

public: 
    BotTelegramException(std::string);

    const char* what() const noexcept override;
};

class BadInitBotTelegramException : public BotTelegramException{
public:
    BadInitBotTelegramException(std::string);
    
};

class BotTelegram{
private:
    std::unordered_map<std::string, TelegramUser> users;
    std::atomic<bool> flag;
    std::thread worker;
    std::string offset;
    std::set<std::string> users_with_keyboard;

    ProductRecommendations observer;
    ProductSearcher searcher;
    PrefixTree tree;
    UserStateMaching MachingState;
    PoolCards pc;

    void check_message();
    void offset_reload();

    void command_start(std::string&&);
    void command_add_card(std::string&&, std::string&&);
    void command_del_card(std::string&&, std::string&&);
    void command_status(std::string&&);
    void command_my_cards(std::string&&);
    void command_forecast(std::string&&, std::string&&);
    void command_recommendations(std::string&&);
    void command_has_discount(std::string&&, std::string&&);

    std::pair<std::string, std::string> get_command_and_data(const std::string& message) noexcept;

    void init_tree() noexcept;

    void stop();

    void load_users_from_db();

    void send_main_keyboard(const std::string& id) const noexcept;

public:
    BotTelegram(std::string, RecType rectype, ProdType prodtype);

    BotTelegram(const BotTelegram&) = delete;
    BotTelegram& operator=(const BotTelegram&) = delete;

    BotTelegram(BotTelegram&&) noexcept;
    BotTelegram& operator=(BotTelegram&&) noexcept;

    template<typename Type>
    void add_user(Type&&);

    template<typename Type>
    void del_user(Type&&);

    template<typename Type>
    bool is_has_user(Type&&) const;

    template<typename Type>
    void notify_all(Type&&) const;

    ~BotTelegram();
};

template<typename Type>
void BotTelegram::add_user(Type&& user){
    if constexpr(!std::is_same<std::decay_t<Type>, TelegramUser>::value)
        throw BotTelegramException("Value-Type must be a TelegramUser\n");

    auto id = user.get_id();
    auto [it, inserted] = users.insert({id, std::forward<Type>(user)});
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
        throw BotTelegramException("Value-type must be a string\n");

    return users.count(user);
}

template<typename Type>
void BotTelegram::notify_all(Type&& notifi) const {
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value)
        throw BotTelegramException("Value-Type must be a string\n");

    for(const auto& obj : users)
        obj.second.notify(notifi);
}

// template<typename Type>
// void BotTelegram::notify_user_added(Type&& user)
// {
//     if constexpr(!std::is_same<std::decay_t<Type>, TelegramUser>::value)
//         throw BotTelegramException("Value-Type must be a TelegramUser\n");

//     for(auto obs : observers){
//         obs->on_user_added(std::forward<Type>(user));
//     }
// }

// template<typename Type>
// void BotTelegram::notify_user_updated(Type&& user)
// {
//     if constexpr(!std::is_same<std::decay_t<Type>, TelegramUser>::value)
//         throw BotTelegramException("Value-Type must be a TelegramUser\n");

//     for(auto obs : observers){
//         obs->on_user_updated(std::forward<Type>(user));
//     }
// }

#endif //_BOT_TELEGRAM_H_