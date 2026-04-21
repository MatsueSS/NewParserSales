#ifndef HI_SQUARE_H
#define HI_SQUARE_H

#include <vector>
#include <exception>
#include <string>

class HiSquareException : public std::exception{
protected:
    std::string msg;

public:
    HiSquareException(std::string msg);
    HiSquareException(const HiSquareException&);

    const char * what() const noexcept override;

};

class EmptyTableHiSquareException : public HiSquareException{
public:
    EmptyTableHiSquareException(std::string msg);

};

class NoDataHiSquareException : public HiSquareException{
public:
    NoDataHiSquareException(std::string msg);

};

class NegativeValueHiSquareException : public HiSquareException{
public:
    NegativeValueHiSquareException(std::string msg);

};

class DifferentLengthHiSquareException : public HiSquareException{
public:
    DifferentLengthHiSquareException(std::string msg);

};

class HiSquare{
public:
    double find_hi_square(const std::vector<std::vector<int>>& table) const;

};

#endif // HI_SQUARE_H