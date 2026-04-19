#include "JsonReader.h"

std::string JsonReader::erase_for_products(std::string&& str) noexcept
{
    size_t pos = str.find(":");
    if(pos != std::string::npos){
        size_t elpos = str.rfind("\"");
        str = str.substr(pos+2);
    }

    return std::move(str);
}

std::string JsonReader::erase_for_messages(std::string&& str) noexcept
{
    size_t pos = str.find(":");
    if(pos != std::string::npos){
        str = str.substr(pos+2, str.size()-9);
        pos = str.find("\"");
        if(pos != std::string::npos){
            size_t elpos = str.rfind("\"");
            str = str.substr(pos+1, elpos-1);
        }
    }

    return std::move(str);
}

JSexception::JSexception(std::string str) : msg(std::move(str)) {}

JSexception::JSexception(const JSexception& obj) : msg(obj.msg) {}

const char * JSexception::what() const noexcept
{
    return msg.c_str();
}

BadValueTypeJSexception::BadValueTypeJSexception(std::string str) : JSexception(std::move(str)) {}