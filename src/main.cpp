#include <iostream>
#include <fstream>

#include "good_funcs.h"
#include "BotTelegram.h"
#include "Interface.h"
#include "PostgresDB.h"
#include "PyLoader.h"
#include "FileMatcher.h"
#include "Matrix.h"
#include "PoolCards.h"
#include "ProductParser.h"
#include "PyAutoClickParser.h"
#include "ModelSelector.h"
#include "GeometricModel.h"
#include "MarkovChain1Model.h"
#include "MarkovChain2Model.h"
#include "HiSquare.h"
#include "IndependenceWeekHypothesis.h"
#include "ROC_AUC.h"
#include "CurlWrapper.h"
#include "TelegramStategy.h"
#include "json.hpp"
#include "ForecastManager.h"
#include <linear.h>

int main(void)
{
    global_init();

    // TelegramUser user(std::move(get_id()));
    // user.notify(std::string("Hello\n"));

    // std::string url = "https://api.telegram.org/bot7866507385ZoyTIJxkAE7laMt-EQA7vIwq0";
    // url = url + "/getUpdates?offset=512291602";
    // std::ofstream file("../res/result_512291602.json");
    // CURL* curl = curl_easy_init();
    // if(curl){
    //     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    //     CURLcode res = curl_easy_perform(curl);
    //     curl_easy_cleanup(curl);
    // }
    // file.close();

    // auto ptr = TelegramSender::get_instance();
    // ptr->call(std::string(""), type_msg::read, std::string("512291604"));

    
    // BotTelegram bot(get_last_offset());
    // while(true){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

//start

    // Interface inter(get_last_offset(), RecType::MATRIX, ProdType::FILE_SEARCHER, TypeParses::PY_AUTOCLICK_PARSER);

    // while(true){
    //     inter.start_process();
    // }

    // check_independence_week();

    // check_independence_season();

//pretest

    // BotTelegram b(get_last_offset(), RecType::MATRIX, std::move(ptr));
    // b.command_recommendations("828404782");

//test

    // FileMatcher m("../sensetive_res/new_dict.txt");
    
    // PrefixTree tree;
    // PostgresDB db;
    // db.connect(get_conn());
    // std::vector<std::vector<std::string>> result = db.fetch(std::string("SELECT DISTINCT title FROM cards"), std::vector<std::string>{});
    // for(auto& vec : result){
    //     tree.add_word(vec[0]);
    // }

    // Reader reader;
    // PyLoader::load("bash -c 'python3 ../py_scripts/pars_perekrestok_sait.py'");
    // PyLoader::load("bash -c 'python3 ../py_scripts/pars_perekrestok_htmp.py'");
    // reader.make_note(get_conn(), "cards_perekrestok", "perekrestok");

    // PostgresDB db;
    // PrefixTree tree;
    // db.connect(get_conn());
    // std::vector<std::vector<std::string>> unique_card = db.fetch(std::string("SELECT DISTINCT title FROM cards;"), std::vector<std::string>{});
    // for(const auto& obj : unique_card){
    //     tree.add_word(obj[0]);
    // }

    // std::string temp = "Ябл";
    // std::string result = tree.give_word_for_prefix(temp);
    // std::cout << result << '\n';

    // 0,1,4,0,2,0,0,2,0,0,0,0,2,1
    // 0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1
    //std::vector<int> sample = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1};
    
    // ModelSelector ms({TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL});
    // ModelSelector::Result r = ms.select_best(discounts);

    // std::cout << r.best_probability << ' ' << r.best_bic << '\n';

    // PostgresDB db;
    // db.connect(get_conn());

    // GeometricModel gm;
    // MarkovChain1Model m1m;
    // MarkovChain2Model m2m;
    // ROC_AUC ra;

    // for(int i : sample){
    //     std::cout << i << ' ';
    // }
    // std::cout << '\n';

    // std::cout << gm.predict_probability(sample) << ' ' << gm.calculate_bic(sample) << '\n';
    // std::cout << m1m.predict_probability(sample) << ' ' << m1m.calculate_bic(sample) << '\n';
    // std::cout << m2m.predict_probability(sample) << ' ' << m2m.calculate_bic(sample) << '\n';
    // int count = 0;
    // double sum = 0;

    // auto prod = db.fetch(std::string("SELECT title FROM products"), std::vector<std::string>{});
    // for(const auto & obj : prod){
    //     auto data = db.fetch(std::string("SELECT DISTINCT ON (date) date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"), std::vector<std::string>{obj[0]});
    //     auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{obj[0]});
    //     if(first_date.empty()) continue;
    //     std::vector<int> sample;
    //     std::chrono::year_month_day ymd = converte_string(first_date[0][0]);
    //     for(int i = 0; i < data.size();){
    //         if(converte_string(data[i][0]) == ymd){
    //             sample.push_back(1);
    //             i++;
    //         } else {
    //             sample.push_back(0);
    //         }
    //         std::chrono::sys_days date = std::chrono::sys_days{ymd};
    //         date += std::chrono::days{7};
    //         std::chrono::year_month_day n_ymd {date};
    //         ymd = n_ymd;
    //     }
    //     if(sample.size() < 28) continue;

    //     std::vector<double> p,q;

    //     for(int i = 18; i < sample.size(); ++i){
    //         std::vector<int> temp;
    //         for(int j = 0; j < i; ++j) temp.push_back(sample[j]);
    //         try{
    //             auto r = gm.predict_probability(temp);
    //             if(sample[i] == 1) p.push_back(1-r);
    //             else q.push_back(1-r);
    //         } catch(std::exception& e) { continue; }
    //     }        

    //     try{
    //         double res = ra.roc_auc(p,q);
    //         count++;
    //         sum+= res;
    //         std::cout << res << ' ';
    //     } catch (std::exception& e) { continue; }
    // }

    // std::cout << '\n' << sum/count << '\n';

    PostgresDB db;
    db.connect(get_conn());
    auto data = db.fetch(std::string("SELECT DISTINCT ON (date) date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"), std::vector<std::string>{"Яблоки Голден"});
    auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{"Яблоки Голден"});
    std::vector<int> sample;
    std::chrono::year_month_day ymd = converte_string(first_date[0][0]);
    for(int i = 0; i < data.size();){
        if(converte_string(data[i][0]) == ymd){
            sample.push_back(1);
            i++;
        } else {
            sample.push_back(0);
        }
        std::chrono::sys_days date = std::chrono::sys_days{ymd};
        date += std::chrono::days{7};
        std::chrono::year_month_day n_ymd {date};
        ymd = n_ymd;
    }

    for(int i : sample) std::cout << i << ' ';
    std::cout << '\n';

    GeometricModel gm;

    std::cout << gm.calculate_bic(sample) << ' ' << gm.predict_probability(sample) << '\n';

    std::vector<double> p,q;

    for(int i = 18; i < sample.size(); ++i){
        std::vector<int> temp;
        for(int j = 0; j < i; ++j) temp.push_back(sample[j]);
        auto r = gm.predict_probability(temp);
        if(sample[i] == 1) p.push_back(1-r);
        else q.push_back(1-r);
    }  

    ROC_AUC ra;

    std::cout << 1-gm.predict_probability(sample) << ' ' << ra.roc_auc(p,q) << '\n';

    // for(double i : p) std::cout << i << ' ';
    // std::cout << '\n';
    // for(double i : q) std::cout << i << ' ';
    // std::cout << '\n';

    // ModelSelector ms({
    //     TypeModel::GEOMETRIC_MODEL,
    //     TypeModel::MARKOV_CHAIN_1_MODEL,
    //     TypeModel::MARKOV_CHAIN_2_MODEL
    // });
    
    // std::vector<int> markov_data = {0,1,0,1,0,1,0,1};
    
    // auto r = ms.select_best(markov_data);

    // PostgresDB db;
    // db.connect(get_conn());

    // std::string title = "Яблоки Голден";
    // auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{title});
    // //auto ymd = converte_string(first_date[0][0]);
    // std::chrono::year_month_day ymd {std::chrono::year{2025}, std::chrono::month{9}, std::chrono::day{6}};

    // auto query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"), std::vector<std::string>{title});

    // std::vector<int> sample;
    // for(int i = 0; i < query_result.size();){
    //     if(converte_string(query_result[i][0]) == ymd){
    //         sample.push_back(1);
    //         i++;
    //     } else {
    //         sample.push_back(0);
    //     }
    //     std::chrono::sys_days date = std::chrono::sys_days{ymd};
    //     date += std::chrono::days{7};
    //     std::chrono::year_month_day n_ymd {date};
    //     ymd = n_ymd;
    // }

    // for(int i : sample)
    //     std::cout << i << ' ';
    // std::cout << '\n';

    // GeometricModel gm;
    // MarkovChain1Model m1m;
    // MarkovChain2Model m2m;

    // std::cout << gm.predict_probability(sample) << ' ' << gm.calculate_bic(sample) << '\n';
    // std::cout << m1m.predict_probability(sample) << ' ' << m1m.calculate_bic(sample) << '\n';
    // std::cout << m2m.predict_probability(sample) << ' ' << m2m.calculate_bic(sample) << '\n';

    // PostgresDB db;
    // db.connect(get_conn());
    // auto products = db.fetch(std::string("SELECT title FROM products"), std::vector<std::string>{});
    // for(const auto& obj : products){
    //     auto title = obj[0];
    //     auto sales = db.fetch(std::string("SELECT * FROM cards WHERE title = $1 AND discount IS NOT NULL"), std::vector<std::string>{title});
    //     if(sales.size() < 3 && sales.size() >= 1){
    //         std::cout << title << '\n';
    //         return 0;
    //     }
    // }

    // ForecastManager fm(std::initializer_list<TypeModel>{TypeModel::GEOMETRIC_MODEL, TypeModel::MARKOV_CHAIN_1_MODEL, TypeModel::MARKOV_CHAIN_2_MODEL});
    // std::cout << fm.get_probability("Яблоки Голден") << '\n';

    global_delete();

    return 0;
}