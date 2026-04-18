#ifndef _TELEGRAM_STRATEGY_H_
#define _TELEGRAM_STRATEGY_H_

#include "CurlWrapper.h"
#include "good_funcs.h"

#include <future>
#include <memory>
#include <fstream>

class TelegramStrategy{
public:
    TelegramStrategy() : cw_ptr(std::make_shared<CurlWrapper>()) 
    { 
        cw_ptr->set_proxy("socks5h://127.0.0.1:1080"); 
        std::ifstream file("../.env");
        file >> token;
    }

    TelegramStrategy(const TelegramStrategy&) = delete;
    TelegramStrategy& operator=(const TelegramStrategy&) = delete;

    TelegramStrategy(TelegramStrategy&&) noexcept = default;
    TelegramStrategy& operator=(TelegramStrategy&&) noexcept = default;

    std::future<CURLcode> write(std::string id, std::string data)
    {
        auto cw_copy = cw_ptr;
        return std::async(std::launch::async, [cw_copy, url = "https://api.telegram.org/bot" + token + "/sendMessage", id = "chat_id="+id+"&text=", data = std::move(data)]
            { return cw_copy->write(url, id, data); });
    }

    CURLcode read(const std::string& offset)
    {
        return cw_ptr->read("https://api.telegram.org/bot" + token + "/getUpdates?offset=" + offset);
    }

    const std::string& get_response()
    {
        return cw_ptr->get_last_response();
    }

    void send_keyboard(std::string id, std::string text, std::string keyboard_json)
    {
            std::string json_data = "{";
            json_data += "\"chat_id\":\"" + id + "\",";
            json_data += "\"text\":\"" + text + "\",";
            json_data += "\"reply_markup\":" + keyboard_json;
            json_data += "}";

        cw_ptr->write_keyboard("https://api.telegram.org/bot" + token + "/sendMessage", id, text, json_data);
    }

private:
    std::shared_ptr<CurlWrapper> cw_ptr;
    std::string token;

};

#endif