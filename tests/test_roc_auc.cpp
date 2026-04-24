#include "doctest.h"
#include "ROC_AUC.h"

TEST_CASE("ROC AUC"){
    ROC_AUC ra;

    SUBCASE("empty samples"){
        CHECK_THROWS(ra.roc_auc({}, {}));
        CHECK_THROWS(ra.roc_auc({1}, {}));
        CHECK_THROWS(ra.roc_auc({}, {1}));
        CHECK_NOTHROW(ra.roc_auc({1}, {1}));
    }

    SUBCASE("bad probability"){
        CHECK_THROWS(ra.roc_auc({-1}, {0}));
        CHECK_THROWS(ra.roc_auc({0}, {-1}));
        CHECK_THROWS(ra.roc_auc({-1}, {-1}));
        CHECK_NOTHROW(ra.roc_auc({1}, {1}));
    }

    SUBCASE("typical samples"){
        std::vector<double> p = {0.5, 0.529412, 0.5, 0.473684, 0.45, 0.428571, 0.458333, 0.461538};
        std::vector<double> q = {0.428571, 0.428571, 0.458333, 0.461538};
        CHECK(ra.roc_auc(p,q) == 0.78125);
    }
}