#ifndef FACTORY_PARSER_H
#define FACTORY_PARSER_H

#include "Parser.h"
#include "PyAutoClickParser.h"
#include "PyHTMLParser.h"

#include <memory>
#include <exception>

class FactoryParser {
public:
    static std::unique_ptr<Parser> create(TypeParses type){
        switch(type){
            case TypeParses::PY_AUTOCLICK_PARSER:
                return std::make_unique<PyAutoClickParser>();
            case TypeParses::PY_HTML_PARSER:
                return std::make_unique<PyHTMLParser>();
            default:
                throw BadTypeFactoryParserException("invalid type TypeParser\n");
        }
    }
};

#endif // FACTORY_PARSER_H