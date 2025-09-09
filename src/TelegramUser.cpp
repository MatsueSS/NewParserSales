#include "TelegramUser.h"

TelegramUserException::TelegramUserException(std::string str) : msg(std::move(str)) {}

const char* TelegramUserException::what() const noexcept { return msg.c_str(); }

TelegramUser::TelegramUser(std::string str) : id(std::move(str)) {}

std::string TelegramUser::get_id() const{
    return id;
}