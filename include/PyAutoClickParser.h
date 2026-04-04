#ifndef _PY_AUTO_CLICK_PARSER_H_
#define _PY_AUTO_CLICK_PARSER_H_

#include "Parser.h"

class PyAutoClickParser : public Parser {
private:
    std::vector<ProductData> parse_temps() const;

public:
    virtual std::vector<ProductData> fetch_product() const override;

};

#endif