#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "TelegramUser.h"

#include <unordered_map>
#include <type_traits>
#include <string>
#include <vector>

class IUserObserver{
public:
    virtual ~IUserObserver() = default;
    virtual void on_user_added(const TelegramUser& user) = 0;
    virtual void on_user_updated(const TelegramUser& user) = 0;
    virtual std::vector<std::string> recommendation(const std::string& id) = 0;
};

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

class Matrix : public IUserObserver{
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> matrix;

public:
    void on_user_added(const TelegramUser& user) override;
    void on_user_updated(const TelegramUser& user) override;    

    Matrix() = default;

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;

    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(Matrix&&) noexcept = default;

    std::vector<std::string> recommendation(const std::string& id);
};

#endif //_MATRIX_LIKE_H_