#ifndef _FACTORY_SEARCHER_H_
#define _FACTORY_SEARCHER_H_

#include "Matcher.h"
#include "FileMatcher.h"

#include <memory>

class FactoryMatcher{
public:
    static std::unique_ptr<Matcher> create(ProdType type, const std::string& filename, std::shared_ptr<PoolCards> ptr_pc) {
        switch(type){
            case ProdType::FILE_SEARCHER:
                return std::make_unique<FileMatcher>(filename, ptr_pc);
            default:
                throw BadTypeMatcherException("Invalid type ProdType\n");
        }
    }
};

#endif