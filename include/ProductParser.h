#ifndef _PRODUCT_PARSER_H_
#define _PRODUCT_PARSER_H_

#include "Parser.h"

#include <memory>

class ProductParser {
public:
    ProductParser() = default;
    ProductParser(std::unique_ptr<Parser> ptr);

    ProductParser(const ProductParser&) = delete;
    ProductParser& operator=(const ProductParser&) = delete;

    ProductParser(ProductParser&& obj) noexcept;
    ProductParser& operator=(ProductParser&& obj) noexcept;

    void set_strategy(std::unique_ptr<Parser> new_strategy) noexcept;

    std::vector<ProductData> fetch_product() const;

private:
    std::unique_ptr<Parser> ptr_parser;

};

#endif