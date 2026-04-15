#include "PoolCards.h"
#include "doctest.h"
#include "PostgresDB.h"
#include "good_funcs.h"

TEST_CASE("pool_cards"){
    PoolCards pc;

    PostgresDB db;
    db.connect(get_conn());
    auto res = db.fetch(std::string("SELECT COUNT(*) FROM products"), std::vector<std::string>{});

    int last_idx = std::stoi(res[0][0]);

    SUBCASE("not exist title"){
        auto r = pc.get_index("1");

        CHECK(r == 0);
    }

    SUBCASE("0 index"){
        auto r = pc.get_title(0);

        CHECK(r.get_title() == "");
    }

    SUBCASE("negative index"){
        auto r = pc.get_title(-1);

        CHECK(r.get_title() == "");
    }

    SUBCASE("add product"){
        pc.add_product("1");
        auto r = pc.get_index("1");
        
        CHECK(r == last_idx+1);
    }

    SUBCASE("check last added"){
        pc.add_product("1");
        auto r = pc.get_title(last_idx+1);

        CHECK(r.get_title() == "1");
    }
}