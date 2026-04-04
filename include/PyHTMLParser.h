#ifndef _PY_HTML_PARSER_H_
#define _PY_HTML_PARSER_H_

#include "Parser.h"

class PyHTMLParser : public Parser{
public:
    virtual std::vector<ProductData> fetch_product() const override;
    
};

#endif