#ifndef INTERFACE_H
#define INTERFACE_H

//This is the interface that runs the bot and controls the time for scripts to run.

#include "BotTelegram.h"
#include "ProductParser.h"

class Interface{
    std::shared_ptr<PoolCards> ptr_pc;
    std::unique_ptr<BotTelegram> ptr;
    ProductParser pr;

    bool control_date() const;
    std::string save_in_bd(std::vector<ProductData>&& obj) const;
    
public:
    Interface(std::string, RecType rectype, ProdType prodtype, TypeParses typeparser);

    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;

    Interface(Interface&&) noexcept = default;
    Interface& operator=(Interface&&) noexcept = default;

    void start_process() const;
};

#endif // INTERFACE_H