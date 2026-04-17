#include "CurlWrapper.h"

CurlWrapperException::CurlWrapperException(std::string msg) : msg(std::move(msg)) {}
CurlWrapperException::CurlWrapperException(const CurlWrapperException& obj) : msg(obj.msg) {}

const char * CurlWrapperException::what() const noexcept { return msg.c_str(); }

BadConnectionCurlWrapperException::BadConnectionCurlWrapperException(std::string msg) : CurlWrapperException(std::move(msg)) {}

NoInitCurlWrapperException::NoInitCurlWrapperException(std::string msg) : CurlWrapperException(std::move(msg)) {}

void global_init()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void global_delete()
{
    curl_global_cleanup();
}

CurlWrapper::CurlWrapper() { build(); }

CurlWrapper::CurlWrapper(CurlWrapper&& obj) noexcept : curl_ptr(std::move(obj.curl_ptr)) {}

CurlWrapper& CurlWrapper::operator=(CurlWrapper&& obj) noexcept
{
    if(this == &obj) return *this;

    curl_ptr = std::move(obj.curl_ptr);
    return *this;
}

void CurlWrapper::build()
{
    CURL* curl = curl_easy_init();
    if(!curl){
        throw BadConnectionCurlWrapperException("Failed to create curl\n");
    }

    curl_ptr = CURLconnPTR(curl, [](CURL* curl){ curl_easy_cleanup(curl); });

    curl_easy_setopt(curl_ptr.get(), CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_WRITEDATA, &response);

}

void CurlWrapper::close() noexcept
{
    if(curl_ptr) curl_ptr.reset();
}

CurlWrapper::~CurlWrapper()
{
    close();
}

bool CurlWrapper::is_connect() const noexcept
{
    return curl_ptr != nullptr;
}

CURLcode CurlWrapper::read(const std::string& url)
{
    if(!is_connect()) throw NoInitCurlWrapperException("curl wasn't initialized\n");

    curl_easy_setopt(curl_ptr.get(), CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_POST, 0L);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl_ptr.get(), CURLOPT_URL, url.c_str());

    response.clear();
    CURLcode res = curl_easy_perform(curl_ptr.get());

    return res;
}

CURLcode CurlWrapper::write(const std::string& url, const std::string& info, const std::string& data)
{
    if(!is_connect()) throw NoInitCurlWrapperException("curl wasn't initialized\n");

    CURLescapePTR encoding (curl_easy_escape(curl_ptr.get(), data.c_str(), data.length()), [](char* ptr){ curl_free(ptr); });
    std::string post_data = info+std::string(encoding.get());

    curl_easy_setopt(curl_ptr.get(), CURLOPT_POST, 1L);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_HTTPGET, 0L);
    curl_easy_setopt(curl_ptr.get(), CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl_ptr.get(), CURLOPT_POSTFIELDSIZE, post_data.size());
    curl_easy_setopt(curl_ptr.get(), CURLOPT_URL, url.c_str());

    response.clear();
    CURLcode res = curl_easy_perform(curl_ptr.get());

    curl_easy_setopt(curl_ptr.get(), CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl_ptr.get(), CURLOPT_POST, 0L);

    return res;
}

const std::string& CurlWrapper::get_last_response() const noexcept
{
    return response;
}

size_t CurlWrapper::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

void CurlWrapper::set_proxy(const std::string& proxy)
{
    if(!is_connect()) throw NoInitCurlWrapperException("curl wasn't initialized\n");

    curl_easy_setopt(curl_ptr.get(), CURLOPT_PROXY, proxy.c_str());
}

long CurlWrapper::get_http_code() const
{
    if(!is_connect()) throw NoInitCurlWrapperException("curl wasn't initialized\n");

    long http_code = 0;
    curl_easy_getinfo(curl_ptr.get(), CURLINFO_RESPONSE_CODE, &http_code);
    return http_code;
}