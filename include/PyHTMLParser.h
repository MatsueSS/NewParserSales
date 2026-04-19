#ifndef PY_HTML_PARSER_H
#define PY_HTML_PARSER_H

#include "Parser.h"

class PyHTMLParser : public Parser{
public:
    virtual std::vector<ProductData> fetch_product() const override;
    
};

#endif // PY_HTML_PARSER_H