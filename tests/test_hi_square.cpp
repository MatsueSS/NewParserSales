#include "HiSquare.h"
#include "doctest.h"

TEST_CASE("HiSquare"){
    SUBCASE("default table"){
        std::vector<std::vector<int>> table = {{10,40}, {15,35}, {20, 30}};
        HiSquare hs;
        auto r = hs.find_hi_square(table);
        CHECK(r == (5.0/3 + 5.0/7 + 0 + 0 + 5.0/3 + 5.0/7));
    }

    SUBCASE("empty table - {{}}"){
        HiSquare hs;
        CHECK_THROWS(hs.find_hi_square({{}}));
    }

    SUBCASE("empty table - {}"){
        HiSquare hs;
        CHECK_THROWS(hs.find_hi_square({}));
    }

    SUBCASE("zero case"){
        HiSquare hs;
        CHECK_THROWS(hs.find_hi_square({{0,0}, {0,0}}));
    }

    SUBCASE("one element"){
        HiSquare hs;
        std::vector<std::vector<int>> v = {{1}};
        CHECK(hs.find_hi_square(v) == 0.0);
    }

    SUBCASE("negative value"){
        HiSquare hs;
        CHECK_THROWS(hs.find_hi_square({{-1}}));
    }

    SUBCASE("different length"){
        HiSquare hs;
        CHECK_THROWS(hs.find_hi_square({{1,1}, {1,1,1}}));
    }

    SUBCASE("zero column sum"){
        HiSquare hs;
        std::vector<std::vector<int>> table = {{10, 0}, {20, 0}};
        CHECK_NOTHROW(hs.find_hi_square(table));
    }

    SUBCASE("zero row sum"){
        HiSquare hs;
        std::vector<std::vector<int>> table = {{0, 10}, {0, 20}};
        CHECK_NOTHROW(hs.find_hi_square(table));
    }

    SUBCASE("single non-zero value"){
        HiSquare hs;
        CHECK(hs.find_hi_square({{1,0}, {0,0}}) >= 0);
    }

    SUBCASE("large numbers"){
        HiSquare hs;
        std::vector<std::vector<int>> table = {{1000000, 2000000}, {3000000, 4000000}};

        auto r = hs.find_hi_square(table);
        CHECK((r >= 0 && r < 100000));
    }

    SUBCASE("independence"){
        HiSquare hs;
        std::vector<std::vector<int>> table = {{10,10}, {10,10}};

        CHECK(hs.find_hi_square(table) == doctest::Approx(0.0).epsilon(0.001));
    }

    SUBCASE("dependence"){
        HiSquare hs;
        std::vector<std::vector<int>> table = {{100, 0}, {0, 100}};

        CHECK(hs.find_hi_square(table) > 100);
    }

    SUBCASE("table 2x3"){
        HiSquare hs;
        std::vector<std::vector<int>> table = {{10,15,20}, {15, 21, 27}};

        auto r = hs.find_hi_square(table);
        CHECK((r > 0 && r < 100));
    }
}