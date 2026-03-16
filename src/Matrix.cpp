#include "Matrix.h"

#include <queue>

MatrixException::MatrixException(std::string str) : msg(std::move(str)) {}

const char* MatrixException::what() const noexcept 
{
    return msg.c_str();
}

BadTypeMatrixException::BadTypeMatrixException(std::string str) : MatrixException(std::move(str)) {}
EmptyResultMatrixException::EmptyResultMatrixException(std::string str) : MatrixException(std::move(str)) {}

void Matrix::add_user(const std::string& id) noexcept
{
    matrix[id] = {};
}

void Matrix::del_user(const std::string& id) noexcept
{
    matrix.erase(id);
}

void Matrix::add_card(const std::string& id, const std::string& card) noexcept
{
    matrix[id].insert(card);
}

void Matrix::del_card(const std::string& id, const std::string& card) noexcept
{
    matrix[id].erase(card);
}

std::vector<std::string> Matrix::recommendation(const std::string& id) const
{
    if(!matrix.count(id))
        throw EmptyResultMatrixException("User must be added\n");
    std::vector<std::string> result;

    const auto& user_cards = matrix.at(id);

    std::unordered_map<std::string, int> freq;
    for(const auto& obj : matrix){
        if(obj.first == id)
            continue;
        for(const auto& card : obj.second){
            if(!user_cards.count(card))
                freq[card]++;
        }
    }

    std::priority_queue<std::pair<int, std::string>> que;
    for(const auto& obj : freq)
        que.push({obj.second, obj.first});

    while(result.size() != 3 && !que.empty()){
        result.push_back(que.top().second);
        que.pop();
    }

    return result;
}