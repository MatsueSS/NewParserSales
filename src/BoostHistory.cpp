#include "BoostHistory.h"

std::size_t BoostHistory::size() const noexcept {
    return history.size();
}

bool BoostHistory::empty() const noexcept {
    return history.empty();
}

const BoostProduct& BoostHistory::operator[](std::size_t idx) const noexcept {
    return history[idx];
}

const BoostProduct& BoostHistory::front() const noexcept {
    return history.front();
}

const BoostProduct& BoostHistory::back() const noexcept {
    return history.back();
}

CItBoostHistory BoostHistory::cbegin() const noexcept {
    return history.cbegin();
}

CItBoostHistory BoostHistory::cend() const noexcept {
    return history.cend();
}