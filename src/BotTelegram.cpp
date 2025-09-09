#include "BotTelegram.h"

#include <thread>
#include <chrono>

BotTelegram::BotTelegram() {
    worker = std::jthread(&BotTelegram::check_message, this);
}

BotTelegramException::BotTelegramException(std::string str) : msg(std::move(str)) {}

const char* BotTelegramException::what() const noexcept{
    return msg.c_str();
}

void BotTelegram::check_message(std::stop_token st) const {
    while(!st.stop_requested()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}