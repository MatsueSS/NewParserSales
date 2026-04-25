#include "doctest.h"
#include "CurlWrapper.h"

#include <iostream>

TEST_CASE("curl wrapper"){
    global_init();

    SUBCASE("build/default constructor"){
        SUBCASE("default use"){
            CurlWrapper cw;
            CHECK(cw.is_connect() == true);
            cw.close();
            CHECK(cw.is_connect() == false);
            cw.build();
            CHECK(cw.is_connect() == true);
        }

        SUBCASE("a lot of build"){
            CurlWrapper cw;
            for(int i = 0; i < 10000; ++i) cw.build();
            CHECK(cw.is_connect() == true);
        }

        SUBCASE("double close"){
            CurlWrapper cw;
            cw.close();
            cw.close();
            CHECK(cw.is_connect() == false);
        }
    }

    SUBCASE("read for exist url"){
        CurlWrapper cw;
        cw.read("http://youtube.com");
        CHECK(cw.get_http_code() == 400);
        CHECK(!cw.get_last_response().empty());
    }

    SUBCASE("read for non-exisr url"){
        CurlWrapper cw;
        cw.read("https://a/");
        CHECK(cw.get_http_code() == 0);
    }

    global_delete();
}