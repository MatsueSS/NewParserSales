#ifndef _MATRIX_H_
#define _MATRIX_H_

//Here is the code that allows you to read recommendations for users

#include "Recommendations.h"

#include <unordered_map>
#include <type_traits>

class MatrixException : public std::exception {
protected:
    std::string msg;

public:
    MatrixException(std::string);

    const char *what() const noexcept override;
};

class BadTypeMatrixException : public MatrixException{
public:
    BadTypeMatrixException(std::string);
};

class EmptyResultMatrixException : public MatrixException{
public:
    EmptyResultMatrixException(std::string);
};

class Matrix : public Recommendations{
public:
    Matrix(std::shared_ptr<PoolCards> ptr_pc, std::shared_ptr<std::unordered_map<std::string, TelegramUser>> matrix);

    virtual void add_user(const std::string& id) override {};
    virtual void del_user(const std::string& id) override {};
    virtual void add_card(const std::string& id, const std::string& card) override {};
    virtual void del_card(const std::string& id, const std::string& card) override {};

    virtual void add_user(std::string&& id) override {};
    virtual void del_user(std::string&& id) override {};
    virtual void add_card(std::string&& id, std::string&& card) override {};
    virtual void del_card(std::string&& id, std::string&& card) override {};

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;

    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(Matrix&&) noexcept = default;

    std::vector<std::string> recommendation(const std::string& id) const override;
};

#endif //_MATRIX_LIKE_H_