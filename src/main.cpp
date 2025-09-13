#include <iostream>
#include <fstream>

#include "good_funcs.h"
#include "BotTelegram.h"

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

    BotTelegram bot("512291608");
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}