#ifndef _FILE_MATCHER_H_
#define _FILE_MATCHER_H_

// Allows you to determine the name of a card by a synonym
// Takes all information from the file

#include "Matcher.h"
#include "PoolCards.h"

#include <memory>

class FileMatcher : public Matcher {
public:
    FileMatcher(const std::string& filename, std::shared_ptr<PoolCards> ptr_pc);

private:
    std::string filename;
    std::shared_ptr<PoolCards> ptr_pc;

    // Divides by character of a string
    std::vector<std::string> parse(std::string&& str);

protected:
    // Load synonyms and cards
    virtual void load_data() override;

};

#endif