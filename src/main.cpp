#include <iostream>
#include "TelegramUser.h"
#include "good_funcs.h"

int main(void)
{
    TelegramUser user(std::move(get_id()));
    user.notify(std::string("Hello\n"));

    return 0;
}