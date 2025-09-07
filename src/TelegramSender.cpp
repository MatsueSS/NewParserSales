#include "TelegramSender.h"

#include <fstream>

TelegramSender* TelegramSender::instance = nullptr;

TelegramSender::TelegramSender()
{
    std::ifstream file("../.env");
    curl_global_init(CURL_GLOBAL_DEFAULT);
    file >> token;
}

TelegramSender* TelegramSender::get_instance() noexcept
{
    if(instance){
        return instance;
    }

    instance = new TelegramSender();
    return instance;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::ofstream* response = static_cast<std::ofstream*>(userp);
    size_t total_size = size*nmemb;
    response->write(static_cast<char*>(contents), total_size);
    return total_size;
}

void TelegramSender::query(std::string id, type_msg type, std::string offset) noexcept
{
    std::string url = "https://api.telegram.org/bot" + token;
    switch (type)
    {
    case type_msg::read:
    {
        url = url + "/getUpdates?offset=" + offset;
        std::ofstream file("../res/result_" + offset + ".json");
        CURL* curl = curl_easy_init();
        if(curl){
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        break;
    }
    case type_msg::send:
    {
        url = url + "/sendMessage";
        CURL* curl = curl_easy_init();
        if(curl){
            char* encoding = curl_easy_escape(curl, offset.c_str(), offset.length());
            std::string post_fields = "chat_id="+id+"&text="+encoding;
            curl_free(encoding);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        break;
    }
    default:
        break;
    }
}

TSexception::TSexception(std::string str) : msg(std::move(str)) {}

TSexception::TSexception(const TSexception& obj) : msg(obj.msg) {}

const char * TSexception::what() const noexcept 
{
    return msg.c_str();
}

BadTypeValueTSexception::BadTypeValueTSexception(std::string str) : TSexception(std::move(str)) {}