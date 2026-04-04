#include "FileMatcher.h"

#include <sstream>
#include <fstream>

FileMatcher::FileMatcher(const std::string& filename, std::shared_ptr<PoolCards> ptr_pc) : Matcher(filename, ptr_pc)
{
    load_data();
}

void FileMatcher::load_data()
{
    std::ifstream file(filename);
    if(!file.is_open())
        return;
    std::string line;
    while(std::getline(file, line)){
        if(line.empty()) continue;

        std::istringstream ss(line);
        std::string first_part, second_part;

        std::getline(ss, first_part, '|');
        std::getline(ss, second_part);

        std::vector<std::string> synonyms = parse(std::move(first_part));
        std::vector<std::string> cards = parse(std::move(second_part));

        std::vector<uint32_t> id_cards;
        for(const auto& obj : cards){
            id_cards.push_back(ptr_pc->get_index(obj));
        }

        for(const std::string& syn : synonyms){
            dict.emplace(syn, id_cards);
        }
    }
    return;
}

std::vector<std::string> FileMatcher::parse(std::string&& str)
{
    std::vector<std::string> result;
    std::istringstream ss(str);
    std::string token;
    while(std::getline(ss, token, ',')){
        result.emplace_back(token);
    }
    return result;
}