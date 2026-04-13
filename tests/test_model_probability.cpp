#include "ModelSelector.h"
#include "doctest.h"

TEST_CASE("ModelSelector"){

    SUBCASE("empty model"){
        ModelSelector ms({});
        CHECK_THROWS(ms.select_best({1,1,1}));
    }

    SUBCASE("1 model"){
        ModelSelector ms({});
        ms.add_model(TypeModel::GEOMETRIC_MODEL);
        auto r = ms.select_best({1,1,1});
        CHECK(r.best_probability == 1.0);
        CHECK(r.name == TypeModel::GEOMETRIC_MODEL);
        CHECK(r.best_bic == log(3));
    }

    SUBCASE("some models"){
        ModelSelector ms({TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL});
        auto r = ms.select_best({1,1,1});
        CHECK(r.best_probability == 1.0);
        CHECK(r.name == TypeModel::MARKOV_CHAIN_2_MODEL);
    }

    SUBCASE("many models"){
        ModelSelector ms({});
        for(int i = 0; i <= 1000; ++i) ms.add_model(TypeModel::GEOMETRIC_MODEL);
        auto r = ms.select_best({1,1,1});
        CHECK(r.best_probability == 1.0);
        CHECK(r.best_bic == log(3));
        CHECK(r.name == TypeModel::GEOMETRIC_MODEL);
    }
}