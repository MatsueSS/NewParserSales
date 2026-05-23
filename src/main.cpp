// #include <iostream>
// #include <fstream>

// #include "good_funcs.h"
// #include "BotTelegram.h"
// #include "Interface.h"
// #include "PostgresDB.h"
// #include "PyLoader.h"
// #include "FileMatcher.h"
// #include "Matrix.h"
// #include "PoolCards.h"
// #include "ProductParser.h"
// #include "PyAutoClickParser.h"
// #include "ModelSelector.h"
// #include "HiSquare.h"
// #include "IndependenceWeekHypothesis.h"
// #include "ROC_AUC.h"
// #include "CurlWrapper.h"
// #include "TelegramStategy.h"
// #include "json.hpp"
// #include "ForecastManager.h"

// #include <linear.h>

// int main(void)
// {
//     global_init();

//     Interface inter(get_last_offset(), RecType::MATRIX, ProdType::FILE_SEARCHER, TypeParses::PY_AUTOCLICK_PARSER);

//     while(true){
//         inter.start_process();
//     }

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
//     // for(int i = 0; i < data.size();){
//     //     if(converte_string(data[i][0]) == ymd){
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

#include <iostream>
#include <vector>
#include <linear.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <tuple>

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
#include "ModelWrapper.h"

using namespace std;

int main() {
    vector<int> data = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1};
    
    vector<double> pattern00_raw, min_run_raw, max_run_raw, kurtosis_raw, autocorr_raw, lag1, lag2,
                    trans01_raw, trans10_raw, sum_raw, w_sum_raw, mode_raw, entropy_raw;  // признаки
    vector<int> y;  // метки

    int window_size = 4;
    using MyExtractor = FeatureExtractor<FeaturePattern00, FeatureKurtosis, FeatureAutocorrLag2, FeatureMinRun, FeatureMaxRun, FeatureTransitions01, FeatureTransitions10, FeatureSum, FeatureWeightSum, FeatureMode, FeatureEntropy>;
    
    for (size_t i = window_size; i < data.size(); i++) {
        lag1.push_back(data[i-1]);
        lag2.push_back(data[i-2]);

        vector<int> window;
        for(size_t j = i - window_size; j < i; ++j){
            window.push_back(data[j]);
        }

        std::vector<double> ext_result = MyExtractor::extract(window);
        
        pattern00_raw.push_back(ext_result[0]);
        kurtosis_raw.push_back(ext_result[1]);
        autocorr_raw.push_back(ext_result[2]);
        min_run_raw.push_back(ext_result[3]);
        max_run_raw.push_back(ext_result[4]);
        trans01_raw.push_back(ext_result[5]);
        trans10_raw.push_back(ext_result[6]);
        sum_raw.push_back(ext_result[7]);
        w_sum_raw.push_back(ext_result[8]);
        mode_raw.push_back(ext_result[9]);
        entropy_raw.push_back(ext_result[10]);

        y.push_back(data[i]);     // метка: текущее значение
    }
    
    int n = y.size();
    
    // Разделяем на train (50%) и test (50%)
    int train_size = n * 1;
    int test_size = n - train_size;
    
    cout << "Всего объектов: " << n << endl;
    cout << "Train: " << train_size << ", Test: " << test_size << endl << endl;

    vector<result_normalize> norm_result = MyExtractor::normalize({pattern00_raw, kurtosis_raw, autocorr_raw, min_run_raw, max_run_raw, trans01_raw, trans10_raw, sum_raw, w_sum_raw, mode_raw, entropy_raw}, train_size, {data[data.size()-4], data[data.size()-3], data[data.size()-2], data[data.size()-1]});

    vector<double> pattern00_norm = norm_result[0].norm_sample;
    vector<double> kurtosis_norm = norm_result[1].norm_sample;
    vector<double> autocorr_norm = norm_result[2].norm_sample;
    vector<double> min_run_norm = norm_result[3].norm_sample;
    vector<double> max_run_norm = norm_result[4].norm_sample;
    vector<double> trans01_norm = norm_result[5].norm_sample;
    vector<double> trans10_norm = norm_result[6].norm_sample;
    vector<double> sum_norm = norm_result[7].norm_sample;
    vector<double> w_sum_norm = norm_result[8].norm_sample;
    vector<double> mode_norm = norm_result[9].norm_sample;
    vector<double> entropy_norm = norm_result[10].norm_sample;

    vector<vector<double>> features = {pattern00_norm, kurtosis_norm, autocorr_norm, min_run_norm, max_run_norm, lag1, lag2,
                                        trans01_norm, trans10_norm, sum_norm, w_sum_norm, mode_norm, entropy_norm};

    vector<tuple<double, int, int>> correlations;
    for(int i = 0; i < features.size(); ++i){
        for(int j = i+1; j < features.size(); ++j){
            double corr = Feature<FeatureAutocorrLag2>::pearson_correlation(features[i], features[j]);
            correlations.push_back(make_tuple(corr, i, j));
        }
    }

    sort(correlations.begin(), correlations.end(), greater<>());

    vector<bool> removed(features.size(), false);
    for(auto& [corr, i, j] : correlations){
        if(corr > 0.5 && !removed[i] && !removed[j]) removed[j] = true;
    }   

    vector<int> selected;
    for(int i = 0; i < features.size() && selected.size() < 3; ++i){
        if(!removed[i]) selected.push_back(i);
    }

    for(int i :  selected) cout << i << ' ';
    cout << '\n';

    vector<result_normalize> new_norm = {norm_result[0]};

    ModelWrapper mw;
    mw.change_C(0.1);
    mw.set_signs(std::move(new_norm), y, train_size);
    mw.train_model();
    cout << mw.get_weight()[0] << '\n';
    cout << mw.get_probability() << '\n';
    cout << mw.get_test_correct() << '\n';
    cout << mw.get_train_correct() << '\n';
    
    // // Подготовка структуры для LIBLINEAR (только train)
    // struct problem prob;
    // prob.l = train_size;
    // prob.n = 1;
    // prob.y = new double[train_size];
    // prob.x = new feature_node*[train_size];
    
    // for (int i = 0; i < train_size; i++) {
    //     prob.x[i] = new feature_node[2];

    //     prob.x[i][0].index = 1;
    //     prob.x[i][0].value = pattern00_norm[i];

    //     prob.x[i][1].index = -1;
    //     prob.y[i] = y[i];
    // }
    
    // // Параметры
    // struct parameter param;
    // param.solver_type = L2R_LR;
    // param.C = 0.1;
    // param.eps = 0.01;
    // param.nr_weight = 0;
    // param.weight_label = NULL;
    // param.weight = NULL;
    // param.p = 0.1;
    // param.init_sol = NULL;
    
    // // Обучение
    // struct model* model_ = train(&prob, &param);
    
    // // ========== ТОЧНОСТЬ НА ОБУЧЕНИИ ==========
    // int train_correct = 0;
    // for (int i = 0; i < train_size; i++) {
    //     double pred_class = predict(model_, prob.x[i]);
    //     if (pred_class == prob.y[i]) train_correct++;
    // }
    // double train_accuracy = 100.0 * train_correct / train_size;
    
    // // ========== ТОЧНОСТЬ НА ТЕСТЕ ==========
    // int test_correct = 0;
    // for (int i = train_size; i < n; i++) {
    //     feature_node test_point[2];

    //     test_point[0].index = 1;
    //     test_point[0].value = pattern00_norm[i];

    //     test_point[1].index = -1;
        
    //     double pred_class = predict(model_, test_point);
    //     if (pred_class == y[i]) test_correct++;
    // }
    // double test_accuracy = 100.0 * test_correct / test_size;

    // // Также можно вывести вероятность для прогноза следующего значения
    // cout << "\n=== ПРОГНОЗ СЛЕДУЮЩЕГО ЗНАЧЕНИЯ ===" << endl;

    // feature_node next_point[2];

    // next_point[0].index = 1;
    // next_point[0].value = norm_result[0].last_norm;

    // next_point[1].index = -1;

    // double next_probs[2];
    // predict_probability(model_, next_point, next_probs);

    // // cout << "Последние 4 значения: " << last4 << " " << last3 << " " << last2 << " " << last1 << endl;
    // // printf("Pattern00: %d (норм: %.3f)\n", last_pattern00, last_pattern00_norm);
    // // printf("Kurtosis: %.4f (норм: %.3f)\n", last_kurtosis, last_kurtosis_norm);
    // // printf("Min_run: %d (норм: %.3f)\n", last_min_run, last_min_run_norm);
    // printf("Вероятность класса 1: %.4f\n", next_probs[1]);
    // printf("Прогноз: %d\n", next_probs[1] >= 0.5 ? 1 : 0);

    // cout << "=== bic ===" << endl;
    // vector<vector<double>> X (n);
    // for(int i = 0; i < n; ++i){
    //     X[i].push_back(pattern00_norm[i]);
    // }
    // cout << Feature<FeatureAutocorrLag2>::calculate_bic(model_, X, y, 1, false) << '\n';
    
    // // Вывод результатов
    // cout << "=== РЕЗУЛЬТАТЫ ===" << endl;
    // // cout << "Коэффициент при признаке (лаг 1): " << model_->w[0] << endl;
    // // cout << "Коэффициент при признаке (лаг 2): " << model_->w[1] << endl;
    // // cout << "Коэффициент при признаке pattern00_normalzie: " << model_->w[0] << endl;
    // // cout << "Коэффициент при признаке autocorr normalize: " << model_->w[1] << endl;
    // // cout << "Коэффициент при признаке min_run: " << model_->w[2] << endl;
    
    // cout << "Точность на обучении: " << train_correct << "/" << train_size 
    //      << " = " << train_accuracy << "%" << endl;
    // cout << "Точность на тесте: " << test_correct << "/" << test_size 
    //      << " = " << test_accuracy << "%" << endl << endl;
    
    // //Диагноз
    // if (train_accuracy > 90 && test_accuracy < 70) {
    //     cout << "⚠️  ПЕРЕОБУЧЕНИЕ! Уменьшите C" << endl;
    // } else if (train_accuracy < 60 && test_accuracy < 60) {
    //     cout << "⚠️  НЕДООБУЧЕНИЕ! Увеличьте C" << endl;
    // } else {
    //     cout << "✅ Модель в порядке" << endl;
    // }

    // cout << "\n=== ЦЕННОСТЬ ПРИЗНАКОВ (по модулю веса) ===" << endl;
    
    // vector<pair<double, string>> importance;
    // importance.push_back({fabs(model_->w[0]), "patetrn00"});
    
    // sort(importance.begin(), importance.end(), greater<pair<double, string>>());
    
    // for (int i = 0; i < 1; i++) {
    //     printf("%d место: %s (|вес| = %.4f)\n", i+1, importance[i].second.c_str(), importance[i].first);
    // }
    
    // // Очистка
    // delete[] prob.y;
    // for (int i = 0; i < train_size; i++) delete[] prob.x[i];
    // delete[] prob.x;
    // free_and_destroy_model(&model_);
    // destroy_param(&param);
    
    return 0;
}