#ifndef _FORECAST_H_
#define _FORECAST_H_

#include <numeric>
#include <exception>
#include <string>

class ForecastException : public std::exception {
protected:
    std::string msg;

public:
    ForecastException(std::string) noexcept;

    const char* what() const noexcept override;

};

class ZeroDivisionForecastException : ForecastException{
public:
    ZeroDivisionForecastException(std::string) noexcept;

};

class Forecast{
public:
    template<typename Container>
    double median(Container&&) const;

    template<typename Container>
    double dispersion(Container&&) const;
};

template<typename Container>
double Forecast::median(Container&& container) const {
    if(container.size() == 0)
        throw ZeroDivisionForecastException("Zero division\n");

    double sum = 0;
    sum = std::accumulate(begin(container), end(container), sum);
    return sum/container.size();
}

template<typename Container>
double Forecast::dispersion(Container&& container) const {
    if(container.size() == 0)
        throw ZeroDivisionForecastException("Zero division\n");

    double mean = median(container);
    double disper = 0;
    for(int i = 0; i < container.size(); ++i){
        disper += (container[i] - mean)*(container[i]-mean);
    }
    return disper/container.size();
}

#endif //_FORECAST_H_