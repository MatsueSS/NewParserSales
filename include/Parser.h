#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <string>
#include <optional>
#include <exception>

class ParserException : public std::exception {
protected:
    std::string msg;

public:
    ParserException(std::string msg);
    ParserException(const ParserException&);

    const char* what() const noexcept override;

};

class NotExistParserException : public ParserException {
public:
    NotExistParserException(std::string msg);
    
}

struct ProductData{
    std::string title;
    std::string price;
    std::optional<std::string> discount;
    std::string date;
};

enum class TypeParses { PY_HTML_PARSER, PY_AUTOCLICK_PARSER };

class Parser{
protected:
    std::vector<ProductData> parse_json(std::string filename) const;
    std::string clean_price(const std::string& s) const noexcept;

public:
    virtual ~Parser() = default;
    virtual std::vector<ProductData> fetch_product() const = 0;
    
};

#endif