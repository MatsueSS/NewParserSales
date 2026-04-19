#ifndef PREFIX_TREE_H
#define PREFIX_TREE_H

#include <map>
#include <memory>
#include <string>

class PrefixTree{
public:
    PrefixTree();

    PrefixTree(const PrefixTree&) = delete;
    PrefixTree& operator=(const PrefixTree&) = delete;

    PrefixTree(PrefixTree&&) noexcept;
    PrefixTree& operator=(PrefixTree&&) noexcept;

    void add_word(const std::string& str) noexcept;
    
    bool has_word(const std::string& str) const noexcept;
    bool has_prefix(const std::string& str) const noexcept;

    std::string give_word_for_prefix(const std::string& str) const noexcept;

private:
    class TreeNode{
    public:
        std::map<std::string, std::unique_ptr<TreeNode>> children;
        bool is_end;

        TreeNode() : is_end(false){ }
    };

    std::unique_ptr<TreeNode> root;

    size_t utf8_char_len(unsigned char first_byte) const {
        if ((first_byte & 0x80) == 0) return 1;
        if ((first_byte & 0xE0) == 0xC0) return 2;
        if ((first_byte & 0xF0) == 0xE0) return 3;
        if ((first_byte & 0xF8) == 0xF0) return 4;
        return 1;
    }

};

#endif // PREFIX_TREE_H