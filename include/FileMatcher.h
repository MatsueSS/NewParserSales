#ifndef _FILE_MATCHER_H_
#define _FILE_MATCHER_H_

#include "Matcher.h"
#include "PoolCards.h"

class FileMatcher : public Matcher {
public:
    FileMatcher(const std::string& filename, const PoolCards& pc);

private:
    std::string filename;
    const PoolCards& pc;

    std::vector<std::string> parse(std::string&& str);

protected:
    virtual void load_data() override;

};

#endif