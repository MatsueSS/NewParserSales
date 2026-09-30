#include <iostream>
#include <fstream>

#include "good_funcs.h"
#include "BotTelegram.h"
#include "Interface.h"
#include "PyLoader.h"
#include "FileMatcher.h"
#include "Matrix.h"
#include "PoolCards.h"
#include "HiSquare.h"
#include "IndependenceWeekHypothesis.h"
#include "ROC_AUC.h"
#include "TelegramStategy.h"
#include "json.hpp"
#include "Wrappers/PostgresDB.h"

#include "Features_regression/FeaturePattern00.h"
#include "Features_regression/FeatureExtractor.h"
#include "Features_regression/FeatureKurtosis.h"
#include "Features_regression/FeatureAutocorrLag2.h"
#include "Features_regression/FeatureMinRun.h"
#include "Features_regression/FeatureMaxRun.h"
#include "Features_regression/FeatureTransitions01.h"
#include "Features_regression/FeatureTransitions10.h"
#include "Features_regression/FeatureSum.h"
#include "Features_regression/FeatureWeightSum.h"
#include "Features_regression/FeatureMode.h"
#include "Features_regression/FeatureEntropy.h"
#include "Features_regression/FeatureLag1.h"
#include "Features_regression/FeatureLag2.h"

#include <linear.h>

void print(std::vector<double> v){
    for(double i : v) std::cout << i << ' ';
    std::cout << '\n';
}

#include <catboost/libs/model_interface/wrapped_calcer.h>

#include <iostream>
#include <string>
#include <vector>

#include <BoostHistory.h>
#include "Features_boost/FeatureBoostCurrentPrice.h"
#include "Features_boost/FeatureBoostPriceChange1.h"
#include "Features_boost/FeatureBoostPriceChangePct1.h"
#include "Features_boost/FeatureBoostPriceMean4.h"
#include "Features_boost/FeatureBoostPriceMin4.h"
#include "Features_boost/FeatureBoostPriceMax4.h"
#include "Features_boost/FeatureBoostPriceMean8.h"
#include "Features_boost/FeatureBoostPriceMean12.h"
#include "Features_boost/FeatureBoostPrevDiscount.h"
#include "Features_boost/FeatureBoostSinceLastDiscount.h"
#include "Features_boost/FeatureBoostDiscountCount4.h"
#include "Features_boost/FeatureBoostDiscountCount8.h"
#include "Features_boost/FeatureBoostDiscountCount12.h"
#include "Features_boost/FeatureBoostDiscountFreq4.h"
#include "Features_boost/FeatureBoostDiscountFreq8.h"
#include "Features_boost/FeatureBoostDiscountFreq12.h"
#include "Features_boost/FeatureBoostLastDiscountInterval.h"
#include "Features_boost/FeatureBoostDiscountPrevValue.h"
#include "Features_boost/FeatureBoostWeek.h"
#include "Features_boost/FeatureBoostMonth.h"
#include "Features_boost/FeatureBoostQuarter.h"
#include "Features_boost/FeatureBoostStd4.h"
#include "Features_boost/FeatureBoostStd8.h"
#include "Features_boost/FeatureBoostStd12.h"

#include "Features_boost/FeatureBoostExtractor.h"

int main(void)
{
    global_init();

    Interface inter(get_last_offset(), RecType::MATRIX, ProdType::FILE_SEARCHER, TypeParses::PY_AUTOCLICK_PARSER);

    while(true){
        inter.start_process();
    }   

    global_delete();

    return 0;
}
