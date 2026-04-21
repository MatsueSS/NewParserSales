#include "HiSquare.h"

#include "Forecast.h"
#include "PostgresDB.h"
#include "good_funcs.h"

HiSquareException::HiSquareException(std::string msg) : msg(std::move(msg)) {}
HiSquareException::HiSquareException(const HiSquareException& obj) : msg(obj.msg) {}

const char * HiSquareException::what() const noexcept { return msg.c_str(); }

EmptyTableHiSquareException::EmptyTableHiSquareException(std::string msg) : HiSquareException(std::move(msg)) {}

NoDataHiSquareException::NoDataHiSquareException(std::string msg) : HiSquareException(std::move(msg)) {}

NegativeValueHiSquareException::NegativeValueHiSquareException(std::string msg) : HiSquareException(std::move(msg)) {}

DifferentLengthHiSquareException::DifferentLengthHiSquareException(std::string msg) : HiSquareException(std::move(msg)) {}

double HiSquare::find_hi_square(const std::vector<std::vector<int>>& table) const
{
    int size_rows = table.size();
    if(table.empty()) throw EmptyTableHiSquareException("table is empty");
    int size_column = table[0].size();
    std::vector<long long> sum_of_rows (size_rows, 0);
    std::vector<long long> sum_of_columns (size_column, 0);
    int sum = 0;

    for(int i = 0; i < size_rows; ++i){
        if(table[i].size() != size_column) throw DifferentLengthHiSquareException("different length of rows - " + std::to_string(0) + " with " + std::to_string(i));
        for(int j = 0; j < size_column; ++j){
            if(table[i][j] < 0) throw NegativeValueHiSquareException("negative value in vector[" + std::to_string(i) + "][" + std::to_string(j) + "] = " + std::to_string(table[i][j]));
            sum_of_rows[i] += table[i][j];
        }
        sum += sum_of_rows[i];
    }

    if(sum == 0) throw NoDataHiSquareException("the table has no data");

    for(int i = 0; i < size_column; ++i){
        for(int j = 0; j < size_rows; ++j){
            sum_of_columns[i] += table[j][i];
        }
    }

    double hi = 0.0;
    
    for(int i = 0; i < size_rows; ++i){
        for(int j = 0; j < size_column; ++j){
            double frequency = (sum_of_rows[i]/static_cast<double>(sum))*sum_of_columns[j];
            double temp = table[i][j]-frequency;
            if(frequency == 0.0) continue;
            hi += (temp/frequency)*temp;
        }
    }

    return hi;
}