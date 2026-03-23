#include "TelegramUser.h"

TelegramUserException::TelegramUserException(std::string str) : msg(std::move(str)) {}

const char* TelegramUserException::what() const noexcept { return msg.c_str(); }

TelegramUser::TelegramUser(std::string str, const PoolCards& converter) : id(std::move(str)), converter(converter) {}

std::string TelegramUser::get_id() const noexcept
{
    return id;
}

const std::unordered_set<uint32_t>& TelegramUser::get_cards() const noexcept
{
    return lovely_product;
}