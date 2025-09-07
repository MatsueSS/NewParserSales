#ifndef _JSON_READER_H_
#define _JSON_READER_H_

#include <vector>
#include <string>
#include <memory>
#include <array>

class JSexception : public std::exception{
protected:
    std::string msg;

public:
    JSexception(std::string);
    JSexception(const JSexception&);

    const char * what() const noexcept override;
};

class BadValueTypeJSexception : public JSexception{
public:
    BadValueTypeJSexception(std::string);

};

enum class type_json{
    products, message
};

class JsonReader{
public:
    template<typename Type>
    static std::vector<std::string> read(Type&&, type_json);

private:
    static std::string erase_for_products(std::string&&) noexcept;
    static std::string erase_for_messages(std::string&&) noexcept;
};

template<typename Type>
std::vector<std::string> JsonReader::read(Type&& query, type_json type)
{
    if constexpr (!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadValueTypeJSexception("Value must be string\n");
    }

    std::array<char, 1024> buffer;   
    std::string str;

    std::vector<std::string> result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(query.c_str(), "r"), pclose);

    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr){
        switch(type){
            case type_json::products:
                result.push_back(JsonReader::erase_for_products(std::string(buffer.data())));
                break;
            case type_json::message:
                result.push_back(JsonReader::erase_for_messages(std::string(buffer.data())));
                break;
            default:
                break;
        }
    }

    return result;
}

#endif //_JSON_READER_H_