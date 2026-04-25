#include "PostgresDB.h"
#include "doctest.h"

#include "good_funcs.h"

#include <thread>

TEST_CASE("Test for postgres db"){
    SUBCASE("check connect"){
        SUBCASE("bad connect"){
            PostgresDB db;
            CHECK_THROWS(db.connect(std::string("")));
        }

        SUBCASE("double connect"){
            PostgresDB db;
            db.connect(get_conn());
            CHECK(db.is_connect() == true);
            db.connect(get_conn());
            CHECK(db.is_connect() == true);
        }

        SUBCASE("forgot connect"){
            PostgresDB db;
            CHECK(db.is_connect() == false);
        }

        SUBCASE("work with close"){
            PostgresDB db;
            db.connect(get_conn());
            CHECK(db.is_connect() == true);
            db.close();
            CHECK(db.is_connect() == false);
            db.connect(get_conn());
            CHECK(db.is_connect() == true);
        }

        SUBCASE("test with 1 thread"){
            PostgresDB db;
    
            std::thread t([&db]() {
                CHECK_NOTHROW(db.connect(get_conn()));
            });
            t.join();
        }

        SUBCASE("connect with threads"){
            PostgresDB db1, db2;
            
            std::thread thr1 ([&db1](){ db1.connect(get_conn()); CHECK(db1.is_connect() == true); });
            std::thread thr2 ([&db2](){ db2.connect(get_conn()); CHECK(db2.is_connect() == true); });

            thr1.join();
            thr2.join();
        }

        SUBCASE("check connect without connect"){
            PostgresDB db;
            CHECK(db.is_connect() == false);
        }

        SUBCASE("double close"){
            PostgresDB db;
            db.close();
            db.close();
            CHECK(db.is_connect() == false);
        }
    }

    SUBCASE("execute"){
        SUBCASE("without connect"){
            PostgresDB db;
            CHECK_THROWS(db.execute(std::string(""), std::vector<std::string>{}));
        }

        SUBCASE("without params"){
            PostgresDB db;
            db.connect(get_conn());
            CHECK_THROWS(db.execute(std::string("INSERT INTO for_tests (title) VALUES ($1)"), std::vector<std::string>{}));
            CHECK_THROWS(db.execute(std::string("INSERT INTO for_tests (id, title) VALUES ($1, $2)"), std::vector<std::string>{"10"}));
        }

        SUBCASE("typical execute"){
            PostgresDB db;
            db.connect(get_conn());
            db.execute(std::string("BEGIN"), std::vector<std::string>{});
            db.execute(std::string("INSERT INTO for_tests (id, title) VALUES ($1, $2)"), std::vector<std::string>{"1", "hello"});
            auto result = db.fetch(std::string("SELECT * FROM for_tests WHERE id = 1 AND title = 'hello';"), std::vector<std::string>{});
            CHECK(!result.empty());
            db.execute(std::string("ROLLBACK"), std::vector<std::string>{});
        }

        SUBCASE("syntax error"){
            PostgresDB db;
            db.connect(get_conn());
            CHECK_THROWS(db.execute(std::string("ISERT INTO for_tests (id, title) VALUES($1, $2);"), std::vector<std::string>{"1", "h"}));
        }
    }

    SUBCASE("check fetch"){
        SUBCASE("without connection"){
            PostgresDB db;
            CHECK_THROWS(db.fetch(std::string("SELECT * FROM preferences"), std::vector<std::string>{}));
        }

        SUBCASE("without params"){
            PostgresDB db;
            db.connect(get_conn());
            CHECK_THROWS(db.fetch(std::string("SELECT * FROM preferences WHERE id = $1;"), std::vector<std::string>{}));
        }

        SUBCASE("syntax error"){
            PostgresDB db;
            db.connect(get_conn());
            CHECK_THROWS(db.fetch(std::string("ELECT * FROM preferences;"), std::vector<std::string>{}));
        }

        SUBCASE("typical fetch"){
            PostgresDB db;
            db.connect(get_conn());
            db.execute(std::string("BEGIN"), std::vector<std::string>{});
            db.execute(std::string("INSERT INTO for_tests (id, title) VALUES ($1, $2)"), std::vector<std::string>{"1", "hello"});
            auto result = db.fetch(std::string("SELECT * FROM for_tests WHERE id = 1 AND title = 'hello';"), std::vector<std::string>{});
            CHECK(!result.empty());
            db.execute(std::string("ROLLBACK"), std::vector<std::string>{});
        }
    }
}