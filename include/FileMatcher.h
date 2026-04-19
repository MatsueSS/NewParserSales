#ifndef FILE_MATCHER_H
#define FILE_MATCHER_H

// Allows you to determine the name of a card by a synonym
// Takes all information from the file

#include "Matcher.h"

class FileMatcher : public Matcher {
public:
    FileMatcher(const std::string& filename, std::shared_ptr<PoolCards> ptr_pc);

private:
    // Divides by character of a string
    std::vector<std::string> parse(std::string&& str);

protected:
    // Load synonyms and cards
    virtual void load_data() override;

};

#endif // FILE_MATCHER_H