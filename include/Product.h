#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <string>

class ProductException : public std::exception{
protected:
    std::string msg;

public:
    explicit ProductException(std::string);
    explicit ProductException(const ProductException&);

    const char * what() const noexcept override;
    
};

class BadTypeProductException : public ProductException{
public:
    explicit BadTypeProductException(std::string);

};

class Product{
public:
    Product() = default;
    
    Product(const std::string& title, uint32_t id);
    Product(std::string&&, uint32_t id);

    Product(const Product&) = default;
    Product& operator=(const Product&) = default;

    Product(Product&&) noexcept = default;
    Product& operator=(Product&&) noexcept = default;

    std::string get_title() const noexcept;
    uint32_t get_id() const noexcept;

    template<typename Data>
    void set_title(Data&&);

    void set_id(uint32_t id) noexcept;

private:
    std::string title;
    uint32_t id;

};

template<typename Data>
void Product::set_title(Data&& title)
{
    if constexpr(!std::is_same<std::decay_t<Data>, std::string>::value)
        throw BadTypeProductException("Value type must be string");

    this->title = std::forward<Data>(title);
}

#endif