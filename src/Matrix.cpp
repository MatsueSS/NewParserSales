#include "Matrix.h"

MatrixException::MatrixException(std::string str) : msg(std::move(str)) {}

const char* MatrixException::what() const noexcept 
{
    return msg.c_str();
}

BadTypeMatrixException::BadTypeMatrixException(std::string str) : MatrixException(std::move(str)) {}
EmptyResultMatrixException::EmptyResultMatrixException(std::string str) : MatrixException(std::move(str)) {}

void Matrix::on_user_added(const TelegramUser& user)
{
    if(!matrix.count(user.get_id())){
        matrix[user.get_id()] = std::unordered_set<std::string>(user.get_cards().begin(), user.get_cards().end());
    }
}

void Matrix::on_user_updated(const TelegramUser& user)
{
    matrix[user.get_id()] = std::unordered_set<std::string>(user.get_cards().begin(), user.get_cards().end());
}

std::vector<std::string> Matrix::recommendation(const std::string& id)
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
    int max_score = 0;
    for (const auto& [card, score] : freq) {
        if (score > max_score) {
            max_score = score;
            result.clear();
            result.push_back(card);
        } else if (score == max_score) {
            result.push_back(card);
        }
    }

    return result;

}