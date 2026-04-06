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

#include "GeometricModel.h"
#include "MarkovChain1Model.h"
#include "MarkovChain2Model.h"

int main(void)
{
    // TelegramUser user(std::move(get_id()));
    // user.notify(std::string("Hello\n"));

    // std::string url = "https://api.telegram.org/bot7866507385:AAERevbOeUZoyTIJxkAE7laMt-EQA7vIwq0";
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

    GeometricModel gm; // 0,1,4,0,2,0,0,2,0,0,0,0,2,1
    MarkovChain1Model mm;
    MarkovChain2Model m2m;
    std::vector<int> discounts = {0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1};
    
    std::cout << gm.calculate_bic(discounts) << ' ' << gm.predict_probability(discounts) << '\n';
    std::cout << mm.calculate_bic(discounts) << ' ' << mm.predict_probability(discounts) << '\n';
    std::cout << m2m.calculate_bic(discounts) << ' ' << m2m.predict_probability(discounts) << '\n';

    return 0;
}