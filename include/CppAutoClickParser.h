#ifndef CPP_AUTOCLICK_PARSER_H
#define CPP_AUTOCLICK_PARSER_H

#include "Parser.h"

class CppAutoClickParser : public Parser {
private:
    std::string get_clipboard_content() const;
    void pull_json(std::vector<ProductData>& pd, const std::string& url) const;

public:
    virtual std::vector<ProductData> fetch_product() const override;
};

#endif // CPP_AUTOCLICK_PARSER_H