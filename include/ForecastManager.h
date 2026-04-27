#ifndef FORECAST_MANAGER
#define FORECAST_MANAGER

#include "ModelSelector.h"
#include "ForecastCache.h"
#include "PostgresDB.h"
#include "good_funcs.h"
#include "ROC_AUC.h"
#include "IndependenceSeasonHypothesis.h"
#include "IndependenceWeekHypothesis.h"

#include <type_traits>
#include <exception>
#include <string>

class ForecastManagerException : public std::exception{
protected:
    std::string msg;

public:
    ForecastManagerException(std::string msg);
    ForecastManagerException(const ForecastManagerException& obj);

    const char * what() const noexcept override;

};

class SmallSampleForecastManagerException : public ForecastManagerException{
public:
    SmallSampleForecastManagerException(std::string str);

};

class ForecastManager{
public:
    template<typename T, typename = std::enable_if_t<
        !std::is_same<std::decay_t<T>, ForecastManager>::value>>
    ForecastManager(T&& list);

    ForecastManager(const ForecastManager&) = delete;
    ForecastManager& operator=(const ForecastManager&) = delete;

    ForecastManager(ForecastManager&&) noexcept;
    ForecastManager& operator=(ForecastManager&&) noexcept;  

    int get_probability(std::string str);

    void reset_cache();

private:
    ModelSelector ms;
    std::unique_ptr<ForecastCache> fc;
    PostgresDB db;
    ROC_AUC ra;
    IndependenceSeasonHypothesis ish;
    IndependenceWeekHypothesis iwh;

    std::vector<int> build_dates_typical(std::string str);
    std::vector<int> build_dates_seasons(std::string str);
    int check_ish(const std::string& str);
    double get_better_probability(const std::vector<int>& sample);
};

template<typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, ForecastManager>::value>>
ForecastManager::ForecastManager(T&& list) : ms(std::forward<T>(list)), fc(std::make_unique<ForecastCache>())
{
    db.connect(get_conn());
}

#endif