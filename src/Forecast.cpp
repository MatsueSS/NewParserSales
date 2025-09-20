#include "Forecast.h"

ForecastException::ForecastException(std::string msg) noexcept : msg(std::move(msg)) {}

const char* ForecastException::what() const noexcept {
    return msg.c_str();
}

ZeroDivisionForecastException::ZeroDivisionForecastException(std::string msg) noexcept : ForecastException(std::move(msg)) {}