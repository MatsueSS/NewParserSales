#ifndef FACTORY_CACHE_H
#define FACTORY_CACHE_H

#include <unordered_map>
#include <string>
#include <shared_mutex>
#include <optional>
#include <list>
#include <exception>

class ForecastCacheException : public std::exception{
protected:
    std::string msg;

public:
    ForecastCacheException(std::string msg) noexcept;
    ForecastCacheException(const ForecastCacheException& obj) noexcept;

    const char * what() const noexcept override;
    
};

class BoundQuitForecastCacheException : public ForecastCacheException{
public:
    BoundQuitForecastCacheException(std::string msg) noexcept;

};

class ForecastCache{
public:
    ForecastCache() = default;

    ForecastCache(const ForecastCache&) = delete;
    ForecastCache& operator=(const ForecastCache&) = delete;

    ForecastCache(ForecastCache&&) noexcept = delete;
    ForecastCache& operator=(ForecastCache&&) noexcept = delete;

    std::optional<double> get(const std::string& title) noexcept;

    void set(const std::string& title, double probability);

    void reset() noexcept;

private:
    struct Cache {
        std::list<std::string>::iterator lru_iterator;
        double probability;
    };

    std::unordered_map<std::string, Cache> cache;
    std::list<std::string> lru_list;

    std::shared_mutex cache_mutex;
    const int max_cache_size = 1000;

};

#endif // FORECAST_CACHE_H