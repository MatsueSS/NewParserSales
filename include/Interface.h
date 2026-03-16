#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//This is the interface that runs the bot and controls the time for scripts to run.

#include "BotTelegram.h"

class Interface{
    std::unique_ptr<BotTelegram> ptr;

    bool control_date() const;
    
public:
    Interface(std::string, std::unique_ptr<Recommendations>, std::unique_ptr<Matcher>);

    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;

    Interface(Interface&&) noexcept = default;
    Interface& operator=(Interface&&) noexcept = default;

    void start_process() const;
};

#endif //_INTERFACE_H_