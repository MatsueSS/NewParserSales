#include "Product.h"

ProductException::ProductException(std::string msg) : msg(std::move(msg)) {}

ProductException::ProductException(const ProductException& obj) : msg(std::move(obj.msg)) {}

const char * ProductException::what() const noexcept { return msg.c_str(); }

BadTypeProductException::BadTypeProductException(std::string msg) : ProductException(std::move(msg)) {}

Product::Product(const std::string& title, uint32_t id) : title(title), id(id) {}

Product::Product(std::string&& title, uint32_t id) : title(std::move(title)), id(id) {}

void Product::set_id(uint32_t id) noexcept
{
    this->id = id;
}

uint32_t Product::get_id() const noexcept
{
    return id;
}

std::string Product::get_title() const noexcept
{
    return title;
}