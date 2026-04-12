#ifndef _Cpp_AUTOCLICK_PARSER_H_
#define _Cpp_AUTOCLICK_PARSER_H_

#include "Parser.h"

class CppAutoClickParser : public Parser {
private:
    std::string get_clipboard_content() const;
    void pull_json(std::vector<ProductData>& pd, const std::string& url) const;

public:
    virtual std::vector<ProductData> fetch_product() const override;
};

#endif