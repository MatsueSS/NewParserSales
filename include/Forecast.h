#ifndef _FORECAST_H_
#define _FORECAST_H_

//Typical methods for working with probability and sampling are described here.

#include <numeric>
#include <exception>
#include <string>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <functional>

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
private:
    double combinations(unsigned n, unsigned k) const;
    double integrate(std::function<double(double)> f, double a, double b, int n = 1000) const;

public:
    template<typename Container>
    auto mean(Container&& container) const
        -> decltype((void)(container.size()), double{});

    template<typename Container>
    auto dispersion(Container&& container) const
        -> decltype((void)(container.size()), double{});

    template<typename Container>
    auto median(Container&& container) const
        -> decltype((void)(container.size()), double{});

    template<typename Container>
    auto geometric_probability(Container&& container, int k) const
        ->decltype((void)(container.size()), double{});

    template<typename Container>
    double binomial_probability(Container&& container, unsigned k) const;

    template<typename Container>
    double poisson_probability(Container&& container, unsigned k) const;

    double exponential_probability(double a, double b, double lambda) const;
    double normal_probability(double a, double b, double mean, double sigma) const;
    double uniform_probability(double a, double b, double x1, double x2) const;
};

template<typename Container>
auto Forecast::mean(Container&& container) const 
    -> decltype((void)(container.size()), double{})
{
    if(container.size() == 0)
        throw ZeroDivisionForecastException("Zero division\n");

    double sum = 0;
    sum = std::accumulate(begin(container), end(container), sum);
    return sum/container.size();
}

template<typename Container>
auto Forecast::median(Container&& container) const
    -> decltype((void)(container.size()), double{})
{
    auto temp = container;
    std::sort(temp.begin(), temp.end());
    if(temp.size() % 2 == 0)
        return (temp[temp.size()/2-1]+temp[temp.size()/2])/2;
    return temp[temp.size()];
}

template<typename Container>
auto Forecast::dispersion(Container&& container) const 
    -> decltype((void)(container.size()), double{})
{
    if(container.size() == 0)
        throw ZeroDivisionForecastException("Zero division\n");

    double mean = mean(container);
    double disper = 0;
    for(const auto& obj: container){
        disper += *obj;
    }
    return disper/container.size();
}

template<typename Container>
auto Forecast::geometric_probability(Container&& container, int k) const 
    ->decltype((void)(container.size()), double{})
{
    double prob = 1/mean(std::forward<Container>(container));
    return std::pow(1-prob, k)*prob;
}

template<typename Container>
double Forecast::binomial_probability(Container&& container, unsigned k) const
{
    int n = container.size();
    double probability = mean(std::forward<Container>(container))/(double)n;
    return combinations(n, k)*std::pow(probability, k)*std::pow(1-probability, n-k);
}

template<typename Container>
double Forecast::poisson_probability(Container&& container, unsigned k) const
{
    int n = container.size();
    double mean_val = mean(std::forward<Container>(container));
    return std::pow(mean_val, k)*exp(-mean_val)/std::tgamma(k+1);
}

#endif //_FORECAST_H_