#ifndef BOOST_HISTORY_H
#define BOOST_HISTORY_H

#include <BoostProduct.h>

#include <vector>
#include <type_traits>

// History must bring only sorted values. Need ORDER BY for SQL-query. And new notes go in back

template<typename T>
concept ConceptBoostProduct = std::same_as<std::remove_cvref_t<T>, BoostProduct>;

using CItBoostHistory = std::vector<BoostProduct>::const_iterator;

class BoostHistory{
private:
    std::vector<BoostProduct> history;

public:
    BoostHistory() noexcept = default;

    template<ConceptBoostProduct T>
    void add_product(T&& product) noexcept {
        history.emplace_back(std::forward<T>(product));
    }

    std::size_t size() const noexcept;
    bool empty() const noexcept;    
    const BoostProduct& operator[](std::size_t idx)const noexcept;
    const BoostProduct& front() const noexcept;
    const BoostProduct& back() const noexcept;
    CItBoostHistory cbegin() const noexcept;
    CItBoostHistory cend() const noexcept;
};

#endif