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

int main() {
    BoostHistory cbh;
    std::chrono::year_month_day ymd(std::chrono::year{2020}, std::chrono::March, std::chrono::day{1});
    BoostProduct pr(1, 1, ymd);
    cbh.add_product(std::move(pr));

    // const std::string modelPath =
    //     "../py_scripts/models_v2/catboost_classifier.cbm";

    // ModelCalcerWrapper classifier(modelPath);

    // std::vector<float> features = {
    //     95.0f,   // price
    //     0.0f,    // price_change_1
    //     0.0f,    // price_change_pct_1
    //     97.5f,   // price_mean_4
    //     2.89f,   // price_std_4
    //     95.0f,   // price_min_4
    //     100.0f,  // price_max_4
    //     98.1f,   // price_mean_8
    //     3.12f,   // price_std_8
    //     99.0f,   // price_mean_12
    //     3.54f,   // price_std_12
    //     0.0f,    // discount_prev_week
    //     2.0f,    // weeks_since_last_discount
    //     1.0f,    // discount_count_last_4
    //     2.0f,    // discount_count_last_8
    //     3.0f,    // discount_count_last_12
    //     0.25f,   // discount_frequency_4
    //     0.25f,   // discount_frequency_8
    //     0.25f,   // discount_frequency_12
    //     2.0f,    // last_discount_interval
    //     10.0f,   // discount_prev_value
    //     37.0f,   // week_of_year
    //     9.0f,    // month
    //     3.0f     // quarter
    // };

    // float rawPrediction = classifier.CalcFlat(features);
    // double probability =
    //     1.0 / (1.0 + std::exp(-rawPrediction));

    // std::cout << "Raw prediction: "
    //           << rawPrediction << '\n';

    // std::cout << "Probability of discount: "
    //           << probability << '\n';

    // return 0;
}

// int main(void)
// {
//     global_init();

//     // Interface inter(get_last_offset(), RecType::MATRIX, ProdType::FILE_SEARCHER, TypeParses::PY_AUTOCLICK_PARSER);

//     // while(true){
//     //     inter.start_process();
//     // }   

//     // std::vector<int> sample = {0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1};

//     // int n = sample.size();

//     // // std::cout << sample.size() << '\n';

//     // LogisticRegressionModel lrm;

//     // auto r = lrm.calculate_bic_with_prob(sample);
//     // std::cout << r.first << ' ' << r.second << '\n';
    
//     // std::vector<int> windows = {6,7,8,9,10};
//     // std::vector<double> C_f = {0.1};
//     // std::vector<double> coef = {0.8};

//     // for(int window_size : windows){
//     //     for(double C : C_f){
//     //         for(double co : coef){
//     //             // using MyExtractor = FeatureExtractor<FeaturePattern00, FeatureKurtosis, FeatureAutocorrLag2, FeatureMinRun,
//     //                     FeatureMaxRun, FeatureLag1, FeatureLag2, FeatureTransitions01, FeatureTransitions10,
//     //                     FeatureSum, FeatureWeightSum, FeatureMode, FeatureEntropy>;

//     //             std::vector<double> pattern00_raw, min_run_raw, max_run_raw, kurtosis_raw, autocorr_raw, lag1, lag2,
//     //                             trans01_raw, trans10_raw, sum_raw, w_sum_raw, mode_raw, entropy_raw;

//     //             std::vector<double> y;

//     //             for (size_t i = window_size; i < sample.size(); i++) {
//     //                 lag1.push_back(sample[i-1]);
//     //                 lag2.push_back(sample[i-2]);

//     //                 std::vector<int> window;
//     //                 for(size_t j = i - window_size; j < i; ++j){
//     //                     window.push_back(sample[j]);
//     //                 }

//     //                 std::vector<double> ext_result = MyExtractor::extract(window);
                    
//     //                 pattern00_raw.push_back(ext_result[0]);
//     //                 kurtosis_raw.push_back(ext_result[1]);
//     //                 autocorr_raw.push_back(ext_result[2]);
//     //                 min_run_raw.push_back(ext_result[3]);
//     //                 max_run_raw.push_back(ext_result[4]);
//     //                 trans01_raw.push_back(ext_result[7]);
//     //                 trans10_raw.push_back(ext_result[8]);
//     //                 sum_raw.push_back(ext_result[9]);
//     //                 w_sum_raw.push_back(ext_result[10]);
//     //                 mode_raw.push_back(ext_result[11]);
//     //                 entropy_raw.push_back(ext_result[12]);

//     //                 y.push_back(sample[i]);
//     //             }   

//     //             ModelWrapper mw;

//     //             int n = y.size();
                
//     //             int train_size = n*co;

//     //             std::vector<int> back_vals;
//     //             for(int i = sample.size()-window_size; i < sample.size(); ++i){
//     //                 back_vals.push_back(sample[i]);
//     //             }

//     //             std::vector<result_normalize> norm_result = MyExtractor::normalize({pattern00_raw, kurtosis_raw, autocorr_raw, min_run_raw, max_run_raw,
//     //                     lag1, lag2, trans01_raw, trans10_raw, sum_raw, w_sum_raw, mode_raw, entropy_raw}, 
//     //                     train_size, back_vals);

//     //             std::vector<result_normalize> new_result = {norm_result[0]};

//     //             mw.change_C(C);

//     //             mw.set_signs(std::move(new_result), y, train_size);

//     //             mw.train_model();

//     //             std::cout << "window_size: " << window_size << '\n';
//     //             std::cout << "C: " << C << '\n';
//     //             std::cout << "coef: " << co << '\n';
//     //             std::cout << "Вероятность класса 1: " << mw.get_probability() << '\n';
//     //             std::cout << "Проверка модели на тестах: " << mw.get_test_correct() << '\n';
//     //             std::cout << "Обучение модели: " << mw.get_train_correct() << '\n';
//     //             std::cout << "ROC-AUC: " << mw.find_roc_auc() << '\n';
//     //             auto r = mw.get_weight();
//     //             for(double w : r) std::cout << w << ' ';
//     //             std::cout << '\n';
//     //         }
//     //     }
//     // }

//     // LogisticRegressionModel lrm;
//     // std::cout << lrm.calculate_bic_with_prob(sample).first << '\n';

//     // GeometricModel gm;
//     // auto r = gm.calculate_bic_with_prob(sample);
//     // std::cout << 1-r.first << ' ' << r.second << '\n';

//     // LogisticRegressionModel lrm;
//     // auto nr = lrm.calculate_bic_with_prob(sample);
//     // std::cout << nr.first << ' ' << nr.second << '\n';

//     // std::vector<double> psk, y;

//     // int ws = 4, n = sample.size();

//     // for(int i = ws; i < n; ++i){
//     //     std::vector<int> window;
//     //     for(int j = i - ws; j < i; ++j){
//     //         window.push_back(sample[j]);
//     //     }

//     //     psk.push_back(Feature<FeatureKurtosis>::compute(window) * Feature<FeatureKurtosis>::compute(window) + Feature<FeatureAutocorrLag2>::compute(window) + Feature<FeatureMinRun>::compute(window) * Feature<FeaturePattern00>::compute(window));
//     //     y.push_back(sample[i]);
//     // }

//     // result_normalize rn = Feature<FeatureKurtosis>::normalize(psk, y.size(), {sample[n-4], sample[n-3], sample[n-2], sample[n-1]});

//     // double ncorr = std::fabs(Feature<FeatureKurtosis>::pearson_correlation(psk, y));
//     // std::cout << ncorr << '\n';


//     //std::vector<int> sample = {0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1,1,0,0,0,0,1};

//     // LogisticRegressionModel lrm;
//     // auto r1 = lrm.calculate_bic_with_prob(sample);
//     // std::cout << r1.first << ' ' << r1.second << '\n';
    
//     // GeometricModel gm;
//     // auto r2 = gm.calculate_bic_with_prob(sample);
//     // std::cout << r2.first << ' ' << r2.second << '\n';

//     // ModelWrapper mw;
//     // mw.change_C(0.1);
//     // mw.set_signs(std::move(norm), y, n-4);
    
//     // double corr = Feature<FeatureKurtosis>::pearson_correlation(norm[0].norm_sample, y);

//     // std::cout << corr << '\n';

//     // check_independence_week();

//     // check_independence_season();


// //pretest

//     // BotTelegram b(get_last_offset(), RecType::MATRIX, std::move(ptr));
//     // b.command_recommendations("828404782");

// //test

//     // FileMatcher m("../sensetive_res/new_dict.txt");
    
//     // PrefixTree tree;
//     // PostgresDB db;
//     // db.connect(get_conn());
//     // std::vector<std::vector<std::string>> result = db.fetch(std::string("SELECT DISTINCT title FROM cards"), std::vector<std::string>{});
//     // for(auto& vec : result){
//     //     tree.add_word(vec[0]);
//     // }

//     // Reader reader;
//     // PyLoader::load("bash -c 'python3 ../py_scripts/pars_perekrestok_sait.py'");
//     // PyLoader::load("bash -c 'python3 ../py_scripts/pars_perekrestok_htmp.py'");
//     // reader.make_note(get_conn(), "cards_perekrestok", "perekrestok");

//     // PostgresDB db;
//     // PrefixTree tree;
//     // db.connect(get_conn());
//     // std::vector<std::vector<std::string>> unique_card = db.fetch(std::string("SELECT DISTINCT title FROM cards;"), std::vector<std::string>{});
//     // for(const auto& obj : unique_card){
//     //     tree.add_word(obj[0]);
//     // }

//     // std::string temp = "Ябл";
//     // std::string result = tree.give_word_for_prefix(temp);
//     // std::cout << result << '\n';

//     // 0,1,4,0,2,0,0,2,0,0,0,0,2,1
//     // 0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1
//     //std::vector<int> sample = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1};
    
//     // ModelSelector ms({TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL});
//     // ModelSelector::Result r = ms.select_best(discounts);

//     // std::cout << r.best_probability << ' ' << r.best_bic << '\n';

//     // PostgresDB db;
//     // db.connect(get_conn());

//     // GeometricModel gm;
//     // MarkovChain1Model m1m;
//     // MarkovChain2Model m2m;
//     // ROC_AUC ra;

//     // for(int i : sample){
//     //     std::cout << i << ' ';
//     // }
//     // std::cout << '\n';

//     // std::cout << gm.predict_probability(sample) << ' ' << gm.calculate_bic(sample) << '\n';
//     // std::cout << m1m.predict_probability(sample) << ' ' << m1m.calculate_bic(sample) << '\n';
//     // std::cout << m2m.predict_probability(sample) << ' ' << m2m.calculate_bic(sample) << '\n';
//     // int count = 0;
//     // double sum = 0;

//     // auto prod = db.fetch(std::string("SELECT title FROM products"), std::vector<std::string>{});
//     // for(const auto & obj : prod){
//     //     auto data = db.fetch(std::string("SELECT DISTINCT ON (date) date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"), std::vector<std::string>{obj[0]});
//     //     auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{obj[0]});
//     //     if(first_date.empty()) continue;
//     //     std::vector<int> sample;
//     //     std::chrono::year_month_day ymd = converte_string(first_date[0][0]);
//     //     for(int i = 0; i < data.size();){
//     //         if(converte_string(data[i][0]) == ymd){
//     //             sample.push_back(1);
//     //             i++;
//     //         } else {
//     //             sample.push_back(0);
//     //         }
//     //         std::chrono::sys_days date = std::chrono::sys_days{ymd};
//     //         date += std::chrono::days{7};
//     //         std::chrono::year_month_day n_ymd {date};
//     //         ymd = n_ymd;
//     //     }
//     //     if(sample.size() < 28) continue;

//     //     std::vector<double> p,q;

//     //     for(int i = 18; i < sample.size(); ++i){
//     //         std::vector<int> temp;
//     //         for(int j = 0; j < i; ++j) temp.push_back(sample[j]);
//     //         try{
//     //             auto r = gm.predict_probability(temp);
//     //             if(sample[i] == 1) p.push_back(1-r);
//     //             else q.push_back(1-r);
//     //         } catch(std::exception& e) { continue; }
//     //     }        

//     //     try{
//     //         double res = ra.roc_auc(p,q);
//     //         count++;
//     //         sum+= res;
//     //         std::cout << res << ' ';
//     //     } catch (std::exception& e) { continue; }
//     // }

//     // std::cout << '\n' << sum/count << '\n';

//     // PostgresDB db;
//     // db.connect(get_conn());
//     // auto data = db.fetch(std::string("SELECT DISTINCT ON (date) date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"), std::vector<std::string>{"Яблоки Голден"});
//     // auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{"Яблоки Голден"});
//     // std::vector<int> sample;
//     // std::chrono::year_month_day ymd = converte_string(first_date[0][0]);

//     // auto second_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date DESC LIMIT 1;"), std::vector<std::string>{"Яблоки Голден"});
//     // std::chrono::year_month_day end = converte_string(second_date[0][0]);

//     // int pos = 0;
//     // while(ymd <= end){
//     //     if(pos < data.size() && converte_string(data[pos][0]) == ymd){
//     //         sample.push_back(1);
//     //         pos++;
//     //     } else {
//     //         sample.push_back(0);
//     //     }
//     //     std::chrono::sys_days date = std::chrono::sys_days{ymd};
//     //     date += std::chrono::days{7};
//     //     std::chrono::year_month_day n_ymd {date};
//     //     ymd = n_ymd;
//     // }

//     // for(int i : sample) std::cout << i << ' ';
//     // std::cout << '\n';

//     // GeometricModel gm;

//     // std::cout << gm.calculate_bic(sample) << ' ' << gm.predict_probability(sample) << '\n';

//     // std::vector<double> p,q;

//     // for(int i = 18; i < sample.size(); ++i){
//     //     std::vector<int> temp;
//     //     for(int j = 0; j < i; ++j) temp.push_back(sample[j]);
//     //     auto r = gm.predict_probability(temp);
//     //     if(sample[i] == 1) p.push_back(1-r);
//     //     else q.push_back(1-r);
//     // }  

//     // ROC_AUC ra;

//     // std::cout << 1-gm.predict_probability(sample) << ' ' << ra.roc_auc(p,q) << '\n';

//     // for(double i : p) std::cout << i << ' ';
//     // std::cout << '\n';
//     // for(double i : q) std::cout << i << ' ';
//     // std::cout << '\n';

//     // ModelSelector ms({
//     //     TypeModel::GEOMETRIC_MODEL,
//     //     TypeModel::MARKOV_CHAIN_1_MODEL,
//     //     TypeModel::MARKOV_CHAIN_2_MODEL
//     // });
    
//     // std::vector<int> markov_data = {0,1,0,1,0,1,0,1};
    
//     // auto r = ms.select_best(markov_data);

//     // PostgresDB db;
//     // db.connect(get_conn());

//     // std::string title = "Яблоки Голден";
//     // auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{title});
//     // //auto ymd = converte_string(first_date[0][0]);
//     // std::chrono::year_month_day ymd {std::chrono::year{2025}, std::chrono::month{9}, std::chrono::day{6}};

//     // auto query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"), std::vector<std::string>{title});

//     // std::vector<int> sample;
//     // for(int i = 0; i < query_result.size();){
//     //     if(converte_string(query_result[i][0]) == ymd){
//     //         sample.push_back(1);
//     //         i++;
//     //     } else {
//     //         sample.push_back(0);
//     //     }
//     //     std::chrono::sys_days date = std::chrono::sys_days{ymd};
//     //     date += std::chrono::days{7};
//     //     std::chrono::year_month_day n_ymd {date};
//     //     ymd = n_ymd;
//     // }

//     // for(int i : sample)
//     //     std::cout << i << ' ';
//     // std::cout << '\n';

//     // GeometricModel gm;
//     // MarkovChain1Model m1m;
//     // MarkovChain2Model m2m;

//     // std::cout << gm.predict_probability(sample) << ' ' << gm.calculate_bic(sample) << '\n';
//     // std::cout << m1m.predict_probability(sample) << ' ' << m1m.calculate_bic(sample) << '\n';
//     // std::cout << m2m.predict_probability(sample) << ' ' << m2m.calculate_bic(sample) << '\n';

//     // PostgresDB db;
//     // db.connect(get_conn());
//     // auto products = db.fetch(std::string("SELECT title FROM products"), std::vector<std::string>{});
//     // for(const auto& obj : products){
//     //     auto title = obj[0];
//     //     auto sales = db.fetch(std::string("SELECT * FROM cards WHERE title = $1 AND discount IS NOT NULL"), std::vector<std::string>{title});
//     //     if(sales.size() < 3 && sales.size() >= 1){
//     //         std::cout << title << '\n';
//     //         return 0;
//     //     }
//     // }

//     // ForecastManager fm(std::initializer_list<TypeModel>{TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL});
//     // std::cout << fm.get_probability("Яблоки Голден") << '\n';

//     global_delete();

//     return 0;
// }