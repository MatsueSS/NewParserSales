#ifndef _TELEGRAM_SENDER_H_
#define _TELEGRAM_SENDER_H_

//constructor read a .env file in fisrt row need token
//The following parameters are supplied for the method call:
//id, type_msg, offset msg

#include <memory>
#include <future>
#include <curl/curl.h>

enum class type_msg{
    read, send
};

class TSexception : public std::exception{
protected:
    std::string msg;

public:
    TSexception(std::string);
    TSexception(const TSexception&);
    
    const char * what() const noexcept override;

};

class BadTypeValueTSexception : public TSexception{
public:
    BadTypeValueTSexception(std::string);
};

class TelegramSender{
public:
    static TelegramSender* get_instance() noexcept;

    TelegramSender(const TelegramSender&) = delete;
    TelegramSender& operator=(const TelegramSender&) = delete;

    //need id, type_msg, and offset
    template<typename Data1, typename Type, typename Data2>
    static std::future<void> call(Data1&& id, Type&& type, Data2&& offset);

private:
    static TelegramSender* instance;
    std::string token;

    TelegramSender();

    void query(std::string id, type_msg type, std::string offset) noexcept;
};

size_t writeCallback(void*, size_t, size_t, void*);

template<typename Data1, typename Type, typename Data2>
std::future<void> TelegramSender::call(Data1&& id, Type&&type, Data2&& offset)
{
    if constexpr (!(std::is_same<std::decay_t<Data1>, std::string>::value && std::is_same<std::decay_t<Data2>, std::string>::value)){
        throw BadTypeValueTSexception("Value must be string\n");
    }

    return std::async(std::launch::async, &TelegramSender::query, 
        get_instance(),
        std::forward<Data1>(id),
        std::forward<Type>(type),
        std::forward<Data2>(offset)
    );
}

#endif //_TELEGRAM_SENDER_H_