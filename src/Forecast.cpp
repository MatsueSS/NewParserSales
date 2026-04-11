#include "Forecast.h"

ForecastException::ForecastException(std::string msg) noexcept : msg(std::move(msg)) {}
ForecastException::ForecastException(const ForecastException& obj) : msg(obj.msg) {}

const char* ForecastException::what() const noexcept {
    return msg.c_str();
}

ZeroDivisionForecastException::ZeroDivisionForecastException(std::string msg) noexcept : ForecastException(std::move(msg)) {}

double Forecast::combinations(unsigned n, unsigned k) const
{
    if(k > n)
        return 0;
    return std::tgamma(n+1)/(std::tgamma(k+1)*std::tgamma(n-k+1));
}

double Forecast::integrate(std::function<double(double)> f, double a, double b, int n) const
{
    double h = (b-a)/n;
    double sum = 0.5*(f(a)+f(b));
    for(int i = 1; i < n; ++i)
        sum += f(a+i*h);
    return sum*h;
}

double Forecast::normal_probability(double a, double b, double mean, double sigma) const
{
    auto normal_f = [mean, sigma](double x){
        double coeff = 1.0/(sigma*std::sqrt(2*M_PI));
        double exponent = -((x - mean)*(x-mean))/(2*sigma*sigma);
        return coeff*std::exp(exponent);
    };
    return integrate(normal_f, a, b);
}

double Forecast::exponential_probability(double a, double b, double lambda) const
{
    if(a < 0)
        a = 0;
    if(b < a)
        return 0.0;
    return std::exp(-lambda*a) - std::exp(-lambda*b);
}

double Forecast::uniform_probability(double a, double b, double x1, double x2) const 
{
    if(x1 < a)
        x1 = a;
    if(x2 > b)
        x2 = b;
    if(x1 > x2)
        return 0.0;
    return (x2-x1)/(b-a);
}