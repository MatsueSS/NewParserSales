#ifndef _CURL_WRAPPER_H_
#define _CURL_WRAPPER_H_

//non thread-safety

#include <curl/curl.h>
#include <memory>
#include <functional>
#include <exception>
#include <string>

using CURLconnDeleter = std::function<void(CURL*)>;
using CURLescapeDeleter = std::function<void(char*)>;

using CURLconnPTR = std::unique_ptr<CURL, CURLconnDeleter>;
using CURLescapePTR = std::unique_ptr<char, CURLescapeDeleter>;

class CurlWrapperException : public std::exception{
protected:
    std::string msg;

public:
    CurlWrapperException(std::string msg);
    CurlWrapperException(const CurlWrapperException& obj);

    const char * what() const noexcept override;

};

class BadConnectionCurlWrapperException : public CurlWrapperException{
public:
    BadConnectionCurlWrapperException(std::string msg);

};

class NoInitCurlWrapperException : public CurlWrapperException{
public:
    NoInitCurlWrapperException(std::string msg);

};

class CurlWrapper {
public:
    CurlWrapper();

    CurlWrapper(const CurlWrapper&) = delete;
    CurlWrapper& operator=(const CurlWrapper&) = delete;

    CurlWrapper(CurlWrapper&& obj) noexcept;
    CurlWrapper& operator=(CurlWrapper&& obj) noexcept;

    void build();
    void close() noexcept;

    CURLcode read(const std::string& url);
    CURLcode write(const std::string& url, const std::string& id, const std::string& data);

    void set_proxy(const std::string& proxy);

    long get_http_code() const;

    const std::string& get_last_response() const noexcept;

    bool is_connect() const noexcept;

    ~CurlWrapper();

private:
    CURLconnPTR curl_ptr;
    std::string response;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);

};

void global_init();
void global_delete();

#endif