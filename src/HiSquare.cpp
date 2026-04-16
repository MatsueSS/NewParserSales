#include "HiSquare.h"

#include "Forecast.h"
#include "PostgresDB.h"
#include "good_funcs.h"

double HiSquare::find_hi_square(const std::vector<std::vector<int>>& table) const noexcept
{
    int size_rows = table.size(), size_column = table[0].size();
    std::vector<int> sum_of_rows (size_rows, 0);
    std::vector<int> sum_of_columns (size_column, 0);
    int sum = 0;

    for(int i = 0; i < size_rows; ++i){
        for(int j = 0; j < size_column; ++j){
            sum_of_rows[i] += table[i][j];
        }
        sum += sum_of_rows[i];
    }

    for(int i = 0; i < size_column; ++i){
        for(int j = 0; j < size_rows; ++j){
            sum_of_columns[i] += table[j][i];
        }
    }

    double hi = 0.0;
    
    for(int i = 0; i < size_rows; ++i){
        for(int j = 0; j < size_column; ++j){
            double frequency = (sum_of_rows[i]*sum_of_columns[j])/(double)sum;
            if(frequency > 0){
                double temp = table[i][j]-frequency;
                hi += (temp*temp)/frequency;
            }
        }
    }

    return hi;
}