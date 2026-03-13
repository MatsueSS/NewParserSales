#include "PrefixTree.h"

PrefixTree::PrefixTree() : root(std::make_unique<TreeNode>()) {}

void PrefixTree::add_word(const std::string& str)
{
    TreeNode* temp_ptr = root.get();
    for(size_t i = 0; i < str.size(); ++i){
        size_t char_len = utf8_char_len(str[i]);
        std::string one_char = str.substr(i, char_len);

        if(!temp_ptr->children.count(one_char)){
            temp_ptr->children[one_char] = std::make_unique<TreeNode>();
        }

        temp_ptr = temp_ptr->children[one_char].get();
        i += char_len;
    }
    temp_ptr->is_end = true;
}

bool PrefixTree::has_prefix(const std::string& str)
{
    TreeNode* temp_ptr = root.get();
    for(size_t i = 0; i < str.size(); ++i){
        size_t char_len = utf8_char_len(str[i]);
        std::string one_char = str.substr(i, char_len);

        auto it = temp_ptr->children.find(one_char);
        if(it == temp_ptr->children.end()){
            return false;
        }

        temp_ptr = it->second.get();
        i += char_len;
    }
    return true;
}

bool PrefixTree::has_word(const std::string& str)
{
    TreeNode* temp_ptr = root.get();
    for(size_t i = 0; i < str.size(); ++i){
        size_t char_len = utf8_char_len(str[i]);
        std::string one_char =str.substr(i, char_len);

        auto it = temp_ptr->children.find(one_char);
        if(it == temp_ptr->children.end()){
            return false;
        }

        temp_ptr = it->second.get();
        i += char_len;  
    }
    return temp_ptr->is_end;
}