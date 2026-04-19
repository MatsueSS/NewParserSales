#include "PrefixTree.h"

#include <vector>
#include <functional>

PrefixTree::PrefixTree() : root(std::make_unique<TreeNode>()) {}

PrefixTree::PrefixTree(PrefixTree&& obj) noexcept : root(std::move(obj.root)) {}

PrefixTree& PrefixTree::operator=(PrefixTree&& obj) noexcept
{
    if(this == &obj) return *this;

    root = std::move(obj.root);
    return *this;
}

void PrefixTree::add_word(const std::string& str) noexcept
{
    TreeNode* temp_ptr = root.get();
    for(size_t i = 0; i < str.size();){
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

bool PrefixTree::has_prefix(const std::string& str) const noexcept
{
    TreeNode* temp_ptr = root.get();
    for(size_t i = 0; i < str.size();){
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

bool PrefixTree::has_word(const std::string& str) const noexcept
{
    TreeNode* temp_ptr = root.get();
    for(size_t i = 0; i < str.size();){
        size_t char_len = utf8_char_len(str[i]);
        std::string one_char = str.substr(i, char_len);

        auto it = temp_ptr->children.find(one_char);
        if(it == temp_ptr->children.end()){
            return false;
        }

        temp_ptr = it->second.get();
        i += char_len;  
    }
    return temp_ptr->is_end;
}

std::string PrefixTree::give_word_for_prefix(const std::string& str) const noexcept
{
    std::string result = str;
    
    if(has_prefix(str)){
        TreeNode* temp = root.get();
        for(size_t i = 0; i < str.size();){
            size_t char_len = utf8_char_len(str[i]);
            std::string one_char = str.substr(i, char_len);

            auto it = temp->children.find(one_char);
            if(it == temp->children.end()){
                return result;
            }

            temp = it->second.get();
            i += char_len;
        }
        
        std::function<bool(TreeNode*, std::string&)> find_word = [&](TreeNode* node, std::string& suffix) -> bool {
                if(node->is_end){
                    return true;
                }
                
                for(const auto& [ch, child] : node->children){
                    suffix += ch;
                    if(find_word(child.get(), suffix)){
                        return true;
                    }
                    suffix.erase(suffix.length() - ch.length());
                }
                
                return false;
            };
        
        std::string suffix;
        if(find_word(temp, suffix)){
            result += suffix;
        }
    }
    
    return result;
}