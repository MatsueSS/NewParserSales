#ifndef _PREFIX_TREE_H_
#define _PREFIX_TREE_H_

#include <map>
#include <memory>
#include <string>

class PrefixTree{
public:
    PrefixTree();

    void add_word(const std::string& str);
    
    bool has_word(const std::string& str);
    bool has_prefix(const std::string& str);

private:
    class TreeNode{
    public:
        std::map<std::string, std::unique_ptr<TreeNode>> children;
        bool is_end;

        TreeNode() : is_end(false){ }
    };

    std::unique_ptr<TreeNode> root;

    size_t utf8_char_len(unsigned char first_byte) {
        if ((first_byte & 0x80) == 0) return 1;
        if ((first_byte & 0xE0) == 0xC0) return 2;
        if ((first_byte & 0xF0) == 0xE0) return 3;
        if ((first_byte & 0xF8) == 0xF0) return 4;
        return 1;
    }

};

#endif