#ifndef _FILE_MATCHER_H_
#define _FILE_MATCHER_H_

#include "Matcher.h"
#include "PoolCards.h"

#include <memory>

class FileMatcher : public Matcher {
public:
    FileMatcher(const std::string& filename, std::shared_ptr<PoolCards> ptr_pc);

private:
    std::string filename;
    std::shared_ptr<PoolCards> ptr_pc;

    std::vector<std::string> parse(std::string&& str);

protected:
    virtual void load_data() override;

};

#endif