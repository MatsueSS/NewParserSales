#include "TelegramUser.h"

TelegramUserException::TelegramUserException(std::string str) : msg(std::move(str)) {}

const char* TelegramUserException::what() const noexcept { return msg.c_str(); }

TelegramUser::TelegramUser(std::string str, std::shared_ptr<PoolCards> ptr_pc) : id(std::move(str)), ptr_pc(ptr_pc) {}

std::string TelegramUser::get_id() const noexcept
{
    return id;
}

const std::unordered_set<uint32_t>& TelegramUser::get_cards() const noexcept
{
    return lovely_product;
}