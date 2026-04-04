#include "ProductParser.h"

ProductParser::ProductParser(std::unique_ptr<Parser> ptr) : ptr_parser(std::move(ptr)) {}

ProductParser::ProductParser(ProductParser&& obj) noexcept : ptr_parser(std::move(obj.ptr_parser)) {}

ProductParser& ProductParser::operator=(ProductParser&& obj) noexcept { ptr_parser = std::move(obj.ptr_parser); return *this; }

void ProductParser::set_strategy(std::unique_ptr<Parser> new_strategy) noexcept
{
    ptr_parser = std::move(new_strategy);
}

std::vector<ProductData> ProductParser::fetch_product() const
{
    return ptr_parser ? ptr_parser->fetch_product() : std::vector<ProductData>{};
}