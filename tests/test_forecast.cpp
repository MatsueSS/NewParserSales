#include "doctest.h"
#include "Forecast.h"

#include <vector>

struct SpyVector{
    std::vector<int> data;
    static int copy_count;  
    static int move_count;

    SpyVector(std::initializer_list<int> list) : data(list) {}
    SpyVector(const SpyVector& obj) : data(obj.data) { copy_count++; }
    SpyVector(SpyVector&& obj) : data(std::move(obj.data)) { move_count++; }
    size_t size() const { return data.size(); }
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    static void reset() { copy_count = 0; move_count = 0; }
    int& operator[](size_t index) { return data[index]; }
};

auto begin(SpyVector& obj){ return obj.begin(); }
auto end(SpyVector& obj){ return obj.end(); }

int SpyVector::copy_count = 0;
int SpyVector::move_count = 0;

TEST_CASE("Forecast::mean"){
    Forecast obj;
    SpyVector v = {1,2,3,4,5};

    SUBCASE("standart sample"){
        std::vector<int> v = {1,2,3};
        CHECK(obj.mean(v) == 2.0);
    }

    SUBCASE("sample with 1 element"){
        std::vector<int> v = {5};
        CHECK(obj.mean(v) == 5.0);
    }

    SUBCASE("negative vals"){
        std::vector<int> v = {-5, 0 ,5};
        CHECK(obj.mean(v) == 0.0);
    }

    SUBCASE("empty sample"){
        std::vector<int> v = {};
        CHECK_THROWS(obj.mean(v));
    }

    SUBCASE("check lvalue"){
        SpyVector::reset();
        double m = obj.mean(v);
        CHECK(SpyVector::copy_count == 0);
        CHECK(SpyVector::move_count == 0);
        CHECK(m == 3.0);        
    }    

    SUBCASE("check rvalue"){
        SpyVector::reset();
        double m = obj.mean(std::move(v));
        CHECK(SpyVector::copy_count == 0);
        CHECK(SpyVector::move_count == 0);
        CHECK(m == 3.0);
    }
}

TEST_CASE("Forecast::dispersion"){
    Forecast obj;
    SpyVector v = {1,2,3,4,5};

    SUBCASE("check lvalue"){
        SpyVector::reset();
        double d = obj.dispersion(v);
        CHECK(SpyVector::copy_count == 0);
        CHECK(SpyVector::move_count == 0);
        CHECK(d == 2.0);
    }

    SUBCASE("check rvalue"){
        SpyVector::reset();
        double d = obj.dispersion(v);
        CHECK(SpyVector::copy_count == 0);
        CHECK(SpyVector::move_count == 0);
        CHECK(d == 2.0);
    }

    SUBCASE("empty sample"){
        CHECK_THROWS(obj.dispersion(std::vector<int>{}));
    }

    SUBCASE("one element"){
        std::vector<int> v = {1};
        CHECK(obj.dispersion(v) == 0.0);
    }

    SUBCASE("negative vals"){
        std::vector<int> v = {-1,-2,-3,-4,-5};
        CHECK(obj.dispersion(v) == 2.0);
    }
}

TEST_CASE("Forecast::median"){
    Forecast obj;
    SpyVector v = {1,2,3,4,5};

    SUBCASE("check lvalue"){
        SpyVector::reset();
        auto d = obj.median(v);
        CHECK(SpyVector::copy_count == 1);
        CHECK(SpyVector::move_count == 0);
        CHECK(d == 3);
    }

    SUBCASE("check rvalue"){
        SpyVector::reset();
        auto d = obj.median(std::move(v));
        CHECK(SpyVector::copy_count == 1);
        CHECK(SpyVector::move_count == 0);
        CHECK(d == 3);
    }

    SUBCASE("empty sample"){
        CHECK_THROWS(obj.median(std::vector<int>{}));
    }

    SUBCASE("odd-size sample"){
        std::vector<int> v = {1,2,3,4,5};
        CHECK(obj.median(v) == 3);
    }

    SUBCASE("even-size sample"){
        std::vector<int> v = {1,2,3,4};
        CHECK(obj.median(v) == 2.5);
    }

    SUBCASE("negative odd-size sample"){
        std::vector<int> v = {-1,-2,-3,-4,-5};
        CHECK(obj.median(v) == -3);
    }

    SUBCASE("negative even-size sample"){
        std::vector<int> v = {-1,-2,-3,-4};
        CHECK(obj.median(v) == -2.5);
    }

    SUBCASE("multiple vals in sample"){
        std::vector<int> v = {2,7,4,6,-3,-8,0};
        CHECK(obj.median(v) == 2);
    }
}

TEST_CASE("Forecast::geometric_distribution"){
    Forecast obj;
    SpyVector v = {1,2,3,4,5};

    SUBCASE("empty sample"){
        std::vector<int> v = {};
        CHECK_THROWS(obj.geometric_probability(v, 0));
    }

    SUBCASE("check rvalue"){
        SpyVector::reset();
        double d = obj.geometric_probability(std::move(v), 0);
        CHECK(SpyVector::copy_count == 0);
        CHECK(SpyVector::move_count == 0);
        CHECK(d == doctest::Approx(1.0/3.0));
    }

    SUBCASE("check lvalue"){
        SpyVector::reset();
        double d = obj.geometric_probability(v, 0);
        CHECK(SpyVector::copy_count == 0);
        CHECK(SpyVector::move_count == 0);
        CHECK(d == doctest::Approx(1.0/3.0));
    }

    SUBCASE("only zeroes"){
        std::vector<int> v = {0,0,0,0};
        CHECK_THROWS(obj.geometric_probability(v, 0));
    }

    SUBCASE("negative probability"){
        std::vector<int> v = {-1,-2,-3,-4,-5};
        CHECK_THROWS(obj.geometric_probability(v, 0));
    }
}