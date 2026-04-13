#include "ForecastCache.h"

#include "good_funcs.h"

#include <mutex>

ForecastCacheException::ForecastCacheException(std::string msg) noexcept : msg(std::move(msg)) {}
ForecastCacheException::ForecastCacheException(const ForecastCacheException& obj) noexcept : msg(obj.msg) {}

const char * ForecastCacheException::what() const noexcept { return msg.c_str(); }

BoundQuitForecastCacheException::BoundQuitForecastCacheException(std::string msg) noexcept : ForecastCacheException(std::move(msg)) {}

std::optional<double> ForecastCache::get(const std::string& title) noexcept
{
    std::unique_lock lock(cache_mutex);
    auto it = cache.find(title);
    if(it == cache.end()) return std::nullopt;
    lru_list.splice(lru_list.begin(), lru_list, it->second.lru_iterator);
    return it->second.probability;
}

void ForecastCache::set(const std::string& title, double probability)
{
    if(0 > probability || probability > 1) throw BoundQuitForecastCacheException("probability muse be in [0,1]");
    std::unique_lock lock(cache_mutex);
    auto it = cache.find(title);
    if(it != cache.end()){
        lru_list.splice(lru_list.begin(), lru_list, it->second.lru_iterator);
        it->second.probability = probability;
    } else {
        lru_list.emplace_front(title);
        cache.emplace(title, Cache{lru_list.begin(), probability});
        if(lru_list.size() > max_cache_size){
            cache.erase(lru_list.back());
            lru_list.pop_back();
        }
    }
}

void ForecastCache::reset() noexcept
{
    std::unique_lock lock(cache_mutex);
    cache.clear();
    lru_list.clear();
}