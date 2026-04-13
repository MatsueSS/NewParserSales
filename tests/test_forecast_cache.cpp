#include "doctest.h"
#include "ForecastCache.h"

#include <vector>
#include <thread>
#include <atomic>

TEST_CASE("ForecastCache::get/set/reset"){
    ForecastCache fc;

    SUBCASE("get with empty list"){
        CHECK(fc.get("1") == std::nullopt);
    }

    SUBCASE("set val"){
        fc.set("1", 1.0);
        CHECK(fc.get("1") == 1.0);
    }

    SUBCASE("reset"){
        fc.reset();
        CHECK(fc.get("1") == std::nullopt);
    }

    SUBCASE("get with vals"){
        fc.set("1", 0);
        fc.set("2", 0.5);
        fc.set("3", 1);
        CHECK(fc.get("1") == 0.0);
        CHECK(fc.get("2") == 0.5);
        CHECK(fc.get("3") == 1.0);
        CHECK(fc.get("4") == std::nullopt);
    }

    SUBCASE("double resel"){
        fc.reset();
        fc.reset();
        CHECK(fc.get("1") == std::nullopt);
    }

    SUBCASE("empty key"){
        fc.set("", 0);
        CHECK(fc.get("") == 0.0);
    }

    SUBCASE("override existing key") {
        fc.set("key", 0.5);
        fc.set("key", 0.9);
        CHECK(fc.get("key") == 0.9);
    }

    SUBCASE("upper bound"){
        CHECK_THROWS(fc.set("1", 1.1));
    }

    SUBCASE("lower bound"){
        CHECK_THROWS(fc.set("1", -0.1));
    }
}

TEST_CASE("ForecastCache::check_limit"){
    ForecastCache fc;

    SUBCASE("check_limit"){
        for(int i = 1; i <= 1000; ++i){
            fc.set(std::to_string(i), 0);
        }
        fc.set("1001", 0);
        CHECK(fc.get("1") == std::nullopt);
    }
}

TEST_CASE("ForecastCache with threads"){
    ForecastCache fc;

    for(int i = 1; i <= 100; ++i){
        fc.set(std::to_string(i), 0);
    }

    SUBCASE("concurrent reads"){
        std::vector<std::thread> readers;
        std::atomic<bool> failed{false};
        
        for(int t = 0; t < 10; ++t) {
            readers.emplace_back([&]() {
                for(int i = 0; i < 1000; ++i) {
                    auto val = fc.get(std::to_string(i % 100));
                    if(val.has_value() && (val.value() < 0 || val.value() > 1)) {
                        failed = true;
                    }
                }
            });
        }
        
        for(auto& t : readers) t.join();
        CHECK(failed == false);
    }

    SUBCASE("concurrent reads and writes"){
        std::vector<std::thread> threads;
        std::atomic<bool> failed{false};
        
        for(int t = 0; t < 10; ++t) {
            threads.emplace_back([&, t]() {
                for(int i = 0; i < 500; ++i) {
                    if(i % 2 == 0) {
                        fc.set("thread_" + std::to_string(t), i * 0.001);
                    } else {
                        auto val = fc.get("thread_" + std::to_string(t));
                        if(val.has_value() && (val.value() < 0 || val.value() > 1)) {
                            failed = true;
                        }
                    }
                }
            });
        }
        
        for(auto& t : threads) t.join();
        CHECK(failed == false);
    }
}

TEST_CASE("ForecastCache - performance") {
    ForecastCache fc;
    
    for(int i = 0; i < 500; ++i) {
        fc.set(std::to_string(i), 0.5);
    }
    
    SUBCASE("many get operations") {
        auto start = std::chrono::steady_clock::now();
        
        for(int i = 0; i < 100000; ++i) {
            fc.get(std::to_string(i % 500));
        }
        
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        CHECK(elapsed.count() < 1000);
    }
}