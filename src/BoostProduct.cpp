#include "BoostProduct.h"

BoostProduct::BoostProduct(uint32_t id, uint32_t price, std::chrono::year_month_day ymd, std::optional<uint32_t> discount)
    : id(id), price(price), date(ymd), discount(discount) {}

uint32_t BoostProduct::get_id() const noexcept { return id; }
uint32_t BoostProduct::get_price() const noexcept { return price; }
std::chrono::year_month_day BoostProduct::get_date() const noexcept { return date; }
std::optional<uint32_t> BoostProduct::get_discount() const noexcept { return discount; }

bool BoostProduct::has_discount() const noexcept { return !(discount == std::nullopt); }