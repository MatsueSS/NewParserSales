#include "Matrix.h"

#include <queue>

MatrixException::MatrixException(std::string str) : msg(std::move(str)) {}

const char* MatrixException::what() const noexcept 
{
    return msg.c_str();
}

BadTypeMatrixException::BadTypeMatrixException(std::string str) : MatrixException(std::move(str)) {}
EmptyResultMatrixException::EmptyResultMatrixException(std::string str) : MatrixException(std::move(str)) {}

Matrix::Matrix(std::shared_ptr<PoolCards> ptr_pc, std::shared_ptr<std::unordered_map<std::string, TelegramUser>> m) : Recommendations(ptr_pc, m)
{}

std::vector<std::string> Matrix::recommendation(const std::string& id) const
{
    if(!matrix->count(id))
        throw EmptyResultMatrixException("User must be added\n");
    std::vector<std::string> result;

    auto user = matrix->find(id);
    const std::unordered_set<uint32_t>& cards = user->second.get_cards();

    std::unordered_map<uint32_t, int> freq;
    for(const auto& obj : *matrix){
        if(obj.first == id)
            continue;
        
        const std::unordered_set<uint32_t>& another_cards = obj.second.get_cards();

        for(const auto& card : another_cards){
            if(!user->second.is_has_product(card))
                freq[card]++;
        }
    }

    std::priority_queue<std::pair<int, uint32_t>> que;
    for(const auto& obj : freq)
        que.push({obj.second, obj.first});

    while(result.size() != 3 && !que.empty()){
        result.push_back(ptr_pc->get_title(que.top().second).get_title());
        que.pop();
    }

    return result;
}