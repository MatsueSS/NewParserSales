#include "ForecastCache.h"

#include "good_funcs.h"

std::optional<double> ForecastCache::get(const std::string& title) noexcept
{
    std::unique_lock lock(cache_mutex);
    auto it = cache.find(title);
    if(it == cache.end()) return std::nullopt;
    lru_list.splice(lru_list.begin(), lru_list, it->second.lru_iterator);
    return it->second.probability;
}

void ForecastCache::set(const std::string& title, double probability) noexcept
{
    std::unique_lock lock(cache_mutex);
    auto it = cache.find(title);
    if(it != cache.end()){
        lru_list.splice(lru_list.begin(), lru_list, it->second.lru_iterator);
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
    cache.clear();
    lru_list.clear();
}