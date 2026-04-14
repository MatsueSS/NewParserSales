#include "ModelSelector.h"
#include "doctest.h"

TEST_CASE("Geometric Model"){
    GeometricModel gm;

    SUBCASE("empty sample"){
        CHECK_THROWS(gm.predict_probability({}));
    }

    SUBCASE("typical sample"){
        CHECK(gm.predict_probability({1,1,1}) == 1.0);
    }

    SUBCASE("bad sample for geometrical probability"){
        CHECK_THROWS(gm.predict_probability({0,0,0}));
    }

    SUBCASE("bic for empty sample"){
        CHECK_THROWS(gm.calculate_bic({}));
    }

    SUBCASE("bic for zeroes sample"){
        CHECK_THROWS(gm.calculate_bic({0,0,0}));
    }

    SUBCASE("typical bic"){
        CHECK(gm.calculate_bic({1,1,1}) == log(3));
    }

    SUBCASE("check name"){
        CHECK(gm.get_name() == TypeModel::GEOMETRIC_MODEL);
    }
}

TEST_CASE("Markov chain 1 model"){
    MarkovChain1Model m1m;

    SUBCASE("empty sample"){
        CHECK_THROWS(m1m.predict_probability({}));
    }

    SUBCASE("inapplicability sample"){
        CHECK_THROWS(m1m.predict_probability({1,1,1,0}));
    }

    SUBCASE("sample with 1 element"){
        CHECK_THROWS(m1m.predict_probability({1}));
    }

    SUBCASE("typical sample"){
        CHECK(m1m.predict_probability({1,1,1}) == 1.0);
        CHECK(m1m.calculate_bic({1,1,1}) == 2*log(2));
    }

    SUBCASE("check name"){
        CHECK(m1m.get_name() == TypeModel::MARKOV_CHAIN_1_MODEL);
    }

    SUBCASE("checking the work"){
        std::vector<int> sample = {1,1,0,0,1,0,1,0,0,0,1,1,0,1,0,1};
        double r = m1m.predict_probability(sample);
        CHECK(r >= 0);
        CHECK(r <= 1);
    }

    SUBCASE("bic for empty sample"){
        CHECK(m1m.calculate_bic({}) == 0.0);
    }
}

TEST_CASE("Markov chain 2 model"){
    MarkovChain2Model m2m;

    SUBCASE("empty sample"){
        CHECK_THROWS(m2m.predict_probability({}));
    }

    SUBCASE("inapplicability sample"){
        CHECK_THROWS(m2m.predict_probability({1,1,1,0}));
    }

    SUBCASE("sample with 1 element and 2 elements"){
        CHECK_THROWS(m2m.predict_probability({1}));
        CHECK_THROWS(m2m.predict_probability({1,1}));
    }
    SUBCASE("typical sample"){
        CHECK(m2m.predict_probability({1,1,1}) == 1.0);
        CHECK(m2m.calculate_bic({1,1,1}) == 0.0);
    }

    SUBCASE("check name"){
        CHECK(m2m.get_name() == TypeModel::MARKOV_CHAIN_2_MODEL);
    }

    SUBCASE("checking the work"){
        std::vector<int> sample = {1,1,0,0,1,0,1,0,0,0,1,1,0,1,0,1};
        double r = m2m.predict_probability(sample);
        CHECK(r >= 0);
        CHECK(r <= 1);
    }

    SUBCASE("bic for empty sample"){
        CHECK(m2m.calculate_bic({}) == 0.0);
    }
}

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

    SUBCASE("no suitable models"){
        ModelSelector ms({TypeModel::MARKOV_CHAIN_1_MODEL});
        CHECK_THROWS(ms.select_best({1,1,1,0}));
    }

    SUBCASE("check model selection for Markov chain 2"){
        ModelSelector ms({
            TypeModel::GEOMETRIC_MODEL,
            TypeModel::MARKOV_CHAIN_1_MODEL,
            TypeModel::MARKOV_CHAIN_2_MODEL
        });
        
        std::vector<int> markov_data = {0,0,1,0,0,1,0,0,1,0,0,1};
        
        auto r = ms.select_best(markov_data);
        CHECK(r.name == TypeModel::MARKOV_CHAIN_2_MODEL);
    }

    SUBCASE("check model selection for Markov chain 1"){
        ModelSelector ms({
            TypeModel::GEOMETRIC_MODEL,
            TypeModel::MARKOV_CHAIN_1_MODEL,
            TypeModel::MARKOV_CHAIN_2_MODEL
        });
        
        std::vector<int> markov_data = {0,1,0,1,0,1,0,1};
        
        auto r = ms.select_best(markov_data);
        CHECK(r.name == TypeModel::MARKOV_CHAIN_1_MODEL);
    }
}