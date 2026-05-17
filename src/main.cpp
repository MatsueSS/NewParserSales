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
// #include "GeometricModel.h"
// #include "MarkovChain1Model.h"
// #include "MarkovChain2Model.h"
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

//     // Interface inter(get_last_offset(), RecType::MATRIX, ProdType::FILE_SEARCHER, TypeParses::PY_AUTOCLICK_PARSER);

//     // while(true){
//     //     inter.start_process();
//     // }

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

//     // std::vector<int> sample_1 = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1};
//     // std::vector<int> sample_2 = converte_zero_to_non_one(sample_1);
//     // lags l = create_lag(sample_2, 3);
//     // auto fe = to_linear(l);
    
//     std::vector<int> sample = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1};

//     std::vector<int> lag1, lag2, lag3, lag4, target;
//     for(int i = 4; i < sample.size(); ++i){
//         lag1.push_back(sample[i-1]);
//         lag2.push_back(sample[i-2]);
//         lag3.push_back(sample[i-3]);
//         lag4.push_back(sample[i-4]);
//         target.push_back(sample[i]);
//     }

//     int n_features = 4;
//     int n_samples = target.size();

//     struct problem prob;
//     prob.l = n_samples;
//     prob.n = n_features;
//     prob.y = (double*)malloc(n_samples * sizeof(double));
//     prob.x = (struct feature_node**)malloc(n_samples * sizeof(struct feature_node*));
//     prob.bias = 1;

//     for(int i = 0; i < n_samples; ++i) {
//         struct feature_node* node = (struct feature_node*)malloc((n_features + 1) * sizeof(struct feature_node));
        
//         node[0].index = 1;
//         node[0].value = lag1[i];
        
//         node[1].index = 2;
//         node[1].value = lag2[i];
        
//         node[2].index = 3;
//         node[2].value = lag3[i];
        
//         node[3].index = 4;
//         node[3].value = lag4[i];
        
//         node[4].index = -1;
        
//         prob.x[i] = node;
//         prob.y[i] = target[i];
//     }
    
//     struct parameter param;
//     param.solver_type = 0;
//     param.C = 1.0;
//     param.eps = 0.01;
//     param.nr_weight = 0;
//     param.weight_label = NULL;
//     param.weight = NULL;
    
//     const char* error_msg = check_parameter(&prob, &param);
//     if(error_msg) {
//         printf("Ошибка в параметрах: %s\n", error_msg);
//         return 1;
//     }

//     struct model* model_ = train(&prob, &param);
    
//     printf("Модель успешно создана!\n");
//     printf("Количество классов: %d\n", model_->nr_class);
    
//     const char* model_file = "model.txt";
//     if(save_model(model_file, model_) == 0) {
//         printf("Модель сохранена в %s\n", model_file);
//     }
    
//     // Пример предсказания
//     std::vector<int> new_lags = {1, 0, 1, 0};
    
//     // Для bias нужно создать дополнительный признак
//     struct feature_node test_nodes[6];  // 4 признака + bias + маркер конца
//     test_nodes[0].index = 1;
//     test_nodes[0].value = new_lags[0];
//     test_nodes[1].index = 2;
//     test_nodes[1].value = new_lags[1];
//     test_nodes[2].index = 3;
//     test_nodes[2].value = new_lags[2];
//     test_nodes[3].index = 4;
//     test_nodes[3].value = new_lags[3];
//     test_nodes[4].index = prob.bias;  // Добавляем bias признак
//     test_nodes[4].value = 1;
//     test_nodes[5].index = -1;  // Маркер конца
    
//     // ИСПРАВЛЕННАЯ ЧАСТЬ - используем predict_probability
//     double* probabilities = (double*)malloc(model_->nr_class * sizeof(double));
//     int predicted_class = predict_probability(model_, test_nodes, probabilities);
    
//     printf("\nПрогноз для новых данных [%d,%d,%d,%d]:\n", 
//            new_lags[0], new_lags[1], new_lags[2], new_lags[3]);
//     printf("Предсказанный класс: %d\n", predicted_class);
//     printf("Вероятности: класс0=%.4f, класс1=%.4f\n", probabilities[0], probabilities[1]);
//     printf("Сумма вероятностей: %.4f\n", probabilities[0] + probabilities[1]);
    
//     // Дополнительно: покажем веса модели для понимания важности лагов
//     printf("\nВеса модели (важность признаков):\n");
//     if(model_->w) {
//         // Для бинарной классификации веса хранятся в model_->w
//         // Первые n_features весов - для класса 0, следующие - для класса 1
//         printf("Признак 1 (Lag1): %.4f\n", model_->w[0]);
//         printf("Признак 2 (Lag2): %.4f\n", model_->w[1]);
//         printf("Признак 3 (Lag3): %.4f\n", model_->w[2]);
//         printf("Признак 4 (Lag4): %.4f\n", model_->w[3]);
//         if(prob.bias >= 0) {
//             printf("Bias (свободный член): %.4f\n", model_->w[4]);
//         }
//     }
    
//     // Очистка
//     free(probabilities);
//     free_and_destroy_model(&model_);
//     destroy_param(&param);
//     free(prob.y);
//     for(int i = 0; i < n_samples; ++i) {
//         free(prob.x[i]);
//     }
//     free(prob.x);

//     return 0;
// }

#include <iostream>
#include <vector>
#include <linear.h>
#include <cmath>
#include <algorithm>
#include <numeric>

#include "Features_regression/FeaturePattern00.h"
#include "Features_regression/FeatureExtractor.h"

using namespace std;

double calculate_kurtosis(const vector<int>& window){
    if(window.size() < 4) return 0.0;

    double mean = 0.0;
    for(int val : window) mean += val;
    mean /= window.size();

    double variance = 0.0;
    for(int val : window) variance += (val-mean)*(val-mean);
    variance /= window.size();

    if(variance < 1e-8) return -3.0;

    double fourth_moment = 0.0;
    for(int val : window) fourth_moment += pow((val-mean)/sqrt(variance), 4);
    fourth_moment /= window.size();

    return fourth_moment - 3.0;
}

double calculate_autocurr_lag2(const vector<int>& window){
    if(window.size() < 4) return 0.0;

    int n = window.size() - 2;

    double mean1 = 0.0, mean2 = 0.0;
    for(int i = 0; i < n; ++i){
        mean1 += window[i];
        mean2 += window[i+2];
    }
    mean1/=n;
    mean2/=n;

    double cov = 0.0, var1 = 0.0, var2 = 0.0;
    for(int i = 0; i < n; ++i){
        cov += (window[i]-mean1)*(window[i+2]-mean2);
        var1 += (window[i]-mean1)*(window[i]-mean1);
        var2 += (window[i+2]-mean2)*(window[i+2]-mean2);
    }
    if(var1 < 1e-8 || var2 < 1e-8) return 0.0;

    return cov/sqrt(var1*var2);
}

int calculate_min_run(const vector<int>& window){
    if(window.empty()) return 0;

    vector<int> runs;
    int current_run = 1;

    for(size_t i = 1; i < window.size(); ++i){
        if(window[i] == window[i-1]) current_run++;
        else {
            runs.push_back(current_run);
            current_run = 1;
        }
    }
    runs.push_back(current_run);

    int min_run = runs[0];
    for(int r : runs) min_run = min(min_run, r);
    return min_run;
}

int calculate_max_run(const vector<int>& window){
    if(window.empty()) return 0;

    vector<int> runs;
    int current_run = 1;

    for(size_t i = 1; i < window.size(); ++i){
        if(window[i] == window[i-1]) current_run++;
        else {
            runs.push_back(current_run);
            current_run = 1;
        }
    }
    runs.push_back(current_run);

    int max_run = runs[0];
    for(int r : runs) max_run = max(max_run, r);
    return max_run;
}

int calculate_transitions(const vector<int>& window, int i1, int i2){
    if(window.size() < 2) return 0;

    int count = 0;
    for(int i = 1; i < window.size(); ++i){
        if(window[i] == i2 && window[i-1] == i1) count++;
    }

    return count;
}

int calculate_sum(const vector<int>& window){
    return accumulate(window.begin(), window.end(), 0);
}

int calculate_weighted_sum(const vector<int>& window){
    int weighted_sum = 0;
    for(int i = 0; i < window.size(); ++i){
        weighted_sum += window[i]*(i+1);
    }
    return weighted_sum;
}

int calculate_mode(const vector<int>& window){
    int sum = accumulate(window.begin(), window.end(), 0);
    return sum > window.size()/2;
}

double calculate_entropy(const vector<int>& window){
    double entropy = 0;
    double p1 = accumulate(window.begin(), window.end(), 0) / (double)window.size();
    double p0 = 1 - p1;
    if(p0 > 0) entropy -= p0 * log(p0);
    if(p1 > 0) entropy -= p1 * log(p1);
    return entropy;
}

double calculate_bic(struct model* model_, const vector<vector<double>>& X, 
                     const vector<double>& y, int n_features, bool has_bias = false) {
    int n = X.size();
    int k = has_bias ? n_features + 1 : n_features;
    
    double log_likelihood = 0.0;
    const double epsilon = 1e-15;
    
    for (int i = 0; i < n; i++) {
        vector<feature_node> nodes(n_features + 1);
        for (int j = 0; j < n_features; j++) {
            nodes[j].index = j + 1;
            nodes[j].value = X[i][j];
        }
        nodes[n_features].index = -1;
        
        double probs[2];
        predict_probability(model_, nodes.data(), probs);
        
        double prob = (y[i] == 1.0) ? probs[1] : probs[0];
        prob = max(epsilon, min(1.0 - epsilon, prob));
        
        log_likelihood += log(prob);
    }
    
    return -2.0 * log_likelihood + k * log(n);
}

int main() {
    vector<int> data = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1};
    
    vector<double> lag1, lag2, pattern00_raw, kurtosis_raw, autocorr_raw, min_run_raw, max_run_raw, trans10, trans01, calc_sum, w_sum, mode_norm, entropy_raw;  // признаки
    vector<double> y;  // метки

    int window_size = 4;
    using MyExtractor = FeatureExtractor<FeaturePattern00>;
    
    for (size_t i = window_size; i < data.size(); i++) {
        lag1.push_back(data[i-1]);
        lag2.push_back(data[i-2]);

        vector<int> window;
        for(size_t j = i - window_size; j < i; ++j){
            window.push_back(data[j]);
        }

        pattern00_raw.push_back(MyExtractor::extract(window)[0]);

        kurtosis_raw.push_back(calculate_kurtosis(window));
        autocorr_raw.push_back(calculate_autocurr_lag2(window));
        min_run_raw.push_back(calculate_min_run(window));
        max_run_raw.push_back(calculate_max_run(window));
        trans10.push_back(calculate_transitions(window, 1, 0));
        trans01.push_back(calculate_transitions(window, 0, 1));
        calc_sum.push_back(calculate_sum(window));
        w_sum.push_back(calculate_weighted_sum(window));
        mode_norm.push_back(calculate_mode(window));
        entropy_raw.push_back(calculate_entropy(window));

        y.push_back(data[i]);     // метка: текущее значение
    }
    
    int n = lag1.size();
    
    // Разделяем на train (50%) и test (50%)
    int train_size = n * 0.5;
    int test_size = n - train_size;
    
    cout << "Всего объектов: " << n << endl;
    cout << "Train: " << train_size << ", Test: " << test_size << endl << endl;

    vector<double> pattern00_norm = MyExtractor::normalize(pattern00_raw, train_size)[0];

    //нормализация kurtosis

    double max_kurtosis = -1e9;
    double min_kurtosis = 1e9;

    for(int i = 0; i < train_size; ++i){
        max_kurtosis = max(max_kurtosis, kurtosis_raw[i]);
        min_kurtosis = min(min_kurtosis, kurtosis_raw[i]);
    }

    vector<double> kurtosis_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_kurtosis - min_kurtosis > 1e-8){
            kurtosis_norm[i] = (kurtosis_raw[i]-min_kurtosis)/(max_kurtosis-min_kurtosis);
        } else {
            kurtosis_norm[i] = 0.5;
        }
    }

    // нормализация автокорреляции с лагом 2

    double max_autocorr = -1e9, min_autocorr = 1e9;
    for(int i = 0; i < train_size; ++i){
        max_autocorr = max(max_autocorr, autocorr_raw[i]);
        min_autocorr = min(min_autocorr, autocorr_raw[i]);
    }

    vector<double> autocorr_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_autocorr - min_autocorr > 1e-8){
            autocorr_norm[i] = (autocorr_raw[i]-min_autocorr)/(max_autocorr-min_autocorr);
        } else {
            autocorr_norm[i] = 0.5;
        }
    }

    // нормализация min_run

    double max_min_run = 0, min_min_run = 100;
    for(int i = 0; i < n; ++i){
        max_min_run = max(max_min_run, min_run_raw[i]);
        min_min_run = min(min_min_run, min_run_raw[i]);
    }

    vector<double> min_run_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_min_run - min_min_run > 1e-8){
            min_run_norm[i] = (min_run_raw[i]-min_min_run)/(max_min_run-min_min_run);
        } else {
            min_run_norm[i] = 0.5;
        }
    }

    // нормализация max_run

    double max_max_run = 0, min_max_run = 100;
    for(int i = 0; i < n; ++i){
        max_max_run = max(max_max_run, max_run_raw[i]);
        min_max_run = min(min_max_run, max_run_raw[i]);
    }

    vector<double> max_run_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_max_run - min_max_run > 1e-8){
            max_run_norm[i] = (max_run_raw[i]-min_max_run)/(max_max_run-min_max_run);
        } else {
            max_run_norm[i] = 0.5;
        }
    }

    // нормализация trans01

    double max_t01 = 0, min_t01 = 100;
    for(int i = 0; i < n; ++i){
        max_t01 = max(max_t01, trans01[i]);
        min_t01 = min(min_t01, trans01[i]);
    }

    vector<double> trans01_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_t01 - min_t01 > 1e-8){
            trans01_norm[i] = (trans01[i]-min_t01)/(max_t01-min_t01);
        } else {
            trans01_norm[i] = 0.5;
        }
    }

    // нормализация trans10

    double max_t10 = 0, min_t10 = 100;
    for(int i = 0; i < n; ++i){
        max_t10 = max(max_t10, trans10[i]);
        min_t10 = min(min_t10, trans10[i]);
    }

    vector<double> trans10_norm(n);
    for(int i = 0 ; i < n; ++i){
        if(max_t10 - min_t10 > 1e-8){
            trans10_norm[i] = (trans10[i]-min_t10)/(max_t10-min_t10);
        } else {
            trans10_norm[i] = 0.5;
        }
    }

    // нормализация calc_sum

    double max_sum = 0, min_sum = 100;
    for(int i = 0; i < n; ++i){
        max_sum = max(max_sum, calc_sum[i]);
        min_sum = min(min_sum, calc_sum[i]);
    }

    vector<double> sum_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_sum - min_sum > 1e-8){
            sum_norm[i] = (calc_sum[i]-min_sum)/(max_sum-min_sum);
        } else {
            sum_norm[i] = 0.5;
        }
    }

    // нормализация w_sum

    double max_w_sum = 0, min_w_sum = 1000;
    for(int i = 0; i < n; ++i){
        max_w_sum = max(max_w_sum, w_sum[i]);
        min_w_sum = min(min_w_sum, w_sum[i]);
    }

    vector<double> w_sum_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_w_sum - min_w_sum > 1e-8){
            w_sum_norm[i] = (w_sum[i]-min_w_sum)/(max_w_sum-min_w_sum);
        } else {
            w_sum_norm[i] = 0.5;
        }
    }

    // нормализация entropy_raw

    double max_entr = INT32_MIN, min_entr = INT32_MAX;
    for(int i = 0; i < n; ++i){
        max_entr = max(max_entr, entropy_raw[i]);
        min_entr = min(min_entr, entropy_raw[i]);
    }

    vector<double> entropy_norm(n);
    for(int i = 0; i < n; ++i){
        if(max_entr - min_entr > 1e-8){
            entropy_norm[i] = (entropy_raw[i]-min_entr)/(max_entr-min_entr);
        } else {
            entropy_norm[i] = 0.5;
        }
    }
    
    // Подготовка структуры для LIBLINEAR (только train)
    struct problem prob;
    prob.l = train_size;
    prob.n = 1;
    prob.y = new double[train_size];
    prob.x = new feature_node*[train_size];
    
    for (int i = 0; i < train_size; i++) {
        prob.x[i] = new feature_node[2];

        prob.x[i][0].index = 1;
        prob.x[i][0].value = pattern00_norm[i];

        prob.x[i][1].index = -1;
        prob.y[i] = y[i];
    }
    
    // Параметры
    struct parameter param;
    param.solver_type = L2R_LR;
    param.C = 0.1;
    param.eps = 0.01;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    param.p = 0.1;
    param.init_sol = NULL;
    
    // Обучение
    struct model* model_ = train(&prob, &param);
    
    // ========== ТОЧНОСТЬ НА ОБУЧЕНИИ ==========
    int train_correct = 0;
    for (int i = 0; i < train_size; i++) {
        double pred_class = predict(model_, prob.x[i]);
        if (pred_class == prob.y[i]) train_correct++;
    }
    double train_accuracy = 100.0 * train_correct / train_size;
    
    // ========== ТОЧНОСТЬ НА ТЕСТЕ ==========
    int test_correct = 0;
    for (int i = train_size; i < n; i++) {
        feature_node test_point[2];

        test_point[0].index = 1;
        test_point[0].value = pattern00_norm[i];

        test_point[1].index = -1;
        
        double pred_class = predict(model_, test_point);
        if (pred_class == y[i]) test_correct++;
    }
    double test_accuracy = 100.0 * test_correct / test_size;

    // После вычисления test_correct, добавьте вывод вероятностей для тестовых объектов

    // cout << "\n=== ВЕРОЯТНОСТИ ДЛЯ ТЕСТОВЫХ ОБЪЕКТОВ ===" << endl;
    // for (int i = train_size; i < n; i++) {
    //     feature_node test_point[6];
    //     test_point[0].index = 1;
    //     test_point[0].value = lag1[i];
    //     test_point[1].index = 2;
    //     test_point[1].value = lag2[i];
    //     test_point[2].index = 3;
    //     test_point[2].value = pattern00_norm[i];
    //     test_point[3].index = 4;
    //     test_point[3].value = kurtosis_norm[i];
    //     test_point[4].index = 5;
    //     test_point[4].value = min_run_norm[i];
    //     test_point[5].index = -1;
        
    //     double probs[2];  // массив для вероятностей: [0] для класса 0, [1] для класса 1
    //     predict_probability(model_, test_point, probs);
        
    //     double pred_class = (probs[1] >= 0.5) ? 1 : 0;
        
    //     printf("Объект %d: Истинный=%d, Предсказанный=%d, Вероятность класса 1=%.4f\n", 
    //         i - train_size + 1, (int)y[i], (int)pred_class, probs[1]);
    // }

    // // Также можно вывести вероятность для прогноза следующего значения
    // cout << "\n=== ПРОГНОЗ СЛЕДУЮЩЕГО ЗНАЧЕНИЯ ===" << endl;

    // // Берём последние 4 значения из data
    // int last1 = data[data.size() - 1];
    // int last2 = data[data.size() - 2];
    // int last3 = data[data.size() - 3];
    // int last4 = data[data.size() - 4];

    // // Считаем pattern00 для последнего окна
    // int last_pattern00 = 0;
    // if (last3 == 0 && last2 == 0) last_pattern00++;
    // if (last2 == 0 && last1 == 0) last_pattern00++;

    // // Нормализуем
    // double last_pattern00_norm;
    // if (max_pattern - min_pattern > 1e-8) {
    //     last_pattern00_norm = (last_pattern00 - min_pattern) / (max_pattern - min_pattern);
    // } else {
    //     last_pattern00_norm = 0.5;
    // }

    // // Считаем kurtosis для последнего окна
    // vector<int> last_window = {last4, last3, last2, last1};
    // double last_kurtosis = calculate_kurtosis(last_window);
    // double last_kurtosis_norm;
    // if (max_kurtosis - min_kurtosis > 1e-8) {
    //     last_kurtosis_norm = (last_kurtosis - min_kurtosis) / (max_kurtosis - min_kurtosis);
    // } else {
    //     last_kurtosis_norm = 0.5;
    // }

    // // Считаем min_run для последнего окна
    // int last_min_run = calculate_min_run(last_window);
    // double last_min_run_norm;
    // if (max_run - min_run > 1e-8) {
    //     last_min_run_norm = (last_min_run - min_run) / (max_run - min_run);
    // } else {
    //     last_min_run_norm = 0.5;
    // }

    // feature_node next_point[6];
    // next_point[0].index = 1;
    // next_point[0].value = last1;
    // next_point[1].index = 2;
    // next_point[1].value = last2;
    // next_point[2].index = 3;
    // next_point[2].value = last_pattern00_norm;
    // next_point[3].index = 4;
    // next_point[3].value = last_kurtosis_norm;
    // next_point[4].index = 5;
    // next_point[4].value = last_min_run_norm;
    // next_point[5].index = -1;

    // double next_probs[2];
    // predict_probability(model_, next_point, next_probs);

    // cout << "Последние 4 значения: " << last4 << " " << last3 << " " << last2 << " " << last1 << endl;
    // printf("Pattern00: %d (норм: %.3f)\n", last_pattern00, last_pattern00_norm);
    // printf("Kurtosis: %.4f (норм: %.3f)\n", last_kurtosis, last_kurtosis_norm);
    // printf("Min_run: %d (норм: %.3f)\n", last_min_run, last_min_run_norm);
    // printf("Вероятность класса 1: %.4f\n", next_probs[1]);
    // printf("Прогноз: %d\n", next_probs[1] >= 0.5 ? 1 : 0);

    cout << "=== bic ===" << endl;
    vector<vector<double>> X (n);
    for(int i = 0; i < n; ++i){
        X[i].push_back(pattern00_norm[i]);
    }
    cout << calculate_bic(model_, X, y, 1, false) << '\n';
    
    // Вывод результатов
    cout << "=== РЕЗУЛЬТАТЫ ===" << endl;
    // cout << "Коэффициент при признаке (лаг 1): " << model_->w[0] << endl;
    // cout << "Коэффициент при признаке (лаг 2): " << model_->w[1] << endl;
    // cout << "Коэффициент при признаке pattern00_normalzie: " << model_->w[0] << endl;
    // cout << "Коэффициент при признаке autocorr normalize: " << model_->w[1] << endl;
    // cout << "Коэффициент при признаке min_run: " << model_->w[2] << endl;
    
    cout << "Точность на обучении: " << train_correct << "/" << train_size 
         << " = " << train_accuracy << "%" << endl;
    cout << "Точность на тесте: " << test_correct << "/" << test_size 
         << " = " << test_accuracy << "%" << endl << endl;
    
    // Диагноз
    if (train_accuracy > 90 && test_accuracy < 70) {
        cout << "⚠️  ПЕРЕОБУЧЕНИЕ! Уменьшите C" << endl;
    } else if (train_accuracy < 60 && test_accuracy < 60) {
        cout << "⚠️  НЕДООБУЧЕНИЕ! Увеличьте C" << endl;
    } else {
        cout << "✅ Модель в порядке" << endl;
    }

    cout << "\n=== ЦЕННОСТЬ ПРИЗНАКОВ (по модулю веса) ===" << endl;
    
    vector<pair<double, string>> importance;
    importance.push_back({fabs(model_->w[0]), "Pattern00"});
    
    sort(importance.begin(), importance.end(), greater<pair<double, string>>());
    
    for (int i = 0; i < 1; i++) {
        printf("%d место: %s (|вес| = %.4f)\n", i+1, importance[i].second.c_str(), importance[i].first);
    }
    
    // Очистка
    delete[] prob.y;
    for (int i = 0; i < train_size; i++) delete[] prob.x[i];
    delete[] prob.x;
    free_and_destroy_model(&model_);
    destroy_param(&param);
    
    return 0;
}