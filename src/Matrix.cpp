#include "Matrix.h"

#include <queue>

MatrixException::MatrixException(std::string str) : msg(std::move(str)) {}

const char* MatrixException::what() const noexcept 
{
    return msg.c_str();
}

BadTypeMatrixException::BadTypeMatrixException(std::string str) : MatrixException(std::move(str)) {}
EmptyResultMatrixException::EmptyResultMatrixException(std::string str) : MatrixException(std::move(str)) {}

Matrix::Matrix(const std::unordered_map<std::string, TelegramUser>& m) : matrix(m)
{}

std::vector<std::string> Matrix::recommendation(const std::string& id) const
{
    if(!matrix.count(id))
        throw EmptyResultMatrixException("User must be added\n");
    std::vector<std::string> result;

    auto user = matrix.find(id);
    const std::unordered_set<std::string>& cards = user->second.get_cards();

    std::unordered_map<std::string, int> freq;
    for(const auto& obj : matrix){
        if(obj.first == id)
            continue;
        
        std::unordered_set<std::string> another_cards = obj.second.get_cards();

        for(const auto& card : another_cards){
            if(!user->second.is_has_product(card))
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