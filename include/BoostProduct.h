#ifndef BOOST_PRODUCT_H
#define BOOST_PRODUCT_H

#include <cstdint>
#include <optional>
#include <chrono>

class BoostProduct{
private:
    uint32_t id;
    uint32_t price;
    std::chrono::year_month_day date;
    std::optional<uint32_t> discount;

public:
    BoostProduct(uint32_t id, uint32_t price, std::chrono::year_month_day date, std::optional<uint32_t> discount = std::nullopt);

    BoostProduct(const BoostProduct& obj) = default;
    BoostProduct(BoostProduct&& obj) noexcept = default;

    BoostProduct& operator=(const BoostProduct& obj) = default;
    BoostProduct& operator=(BoostProduct&& obj) noexcept = default;

    uint32_t get_id() const noexcept;
    uint32_t get_price() const noexcept;
    std::chrono::year_month_day get_date() const noexcept;
    std::optional<uint32_t> get_discount() const noexcept;

    bool has_discount() const noexcept;
};

#endif