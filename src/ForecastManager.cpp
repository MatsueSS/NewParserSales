#include "ForecastManager.h"

#include <chrono>

ForecastManagerException::ForecastManagerException(std::string str) : msg(std::move(str)) {}
ForecastManagerException::ForecastManagerException(const ForecastManagerException& obj) : msg(obj.msg) {}

const char * ForecastManagerException::what() const noexcept { return msg.c_str(); }

SmallSampleForecastManagerException::SmallSampleForecastManagerException(std::string str) : ForecastManagerException(std::move(str)) {}

ForecastManager::ForecastManager(ForecastManager&& obj) noexcept : ms(std::move(obj.ms)), fc(std::move(obj.fc)), db(std::move(obj.db)), ra(std::move(obj.ra)) {}
ForecastManager& ForecastManager::operator=(ForecastManager&& obj) noexcept
{
    if(this == &obj) return *this;

    ms = std::move(obj.ms);
    fc = std::move(obj.fc);
    db = std::move(obj.db);
    ra = std::move(obj.ra);
    return *this;
}

std::vector<int> ForecastManager::build_dates(std::string str)
{
    std::vector<std::vector<std::string>> query_result;
    try{
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{str});
    } catch(BadConnectionDBexception& e){
        db.connect(get_conn());
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{str});
    } catch(ErrorQueryResultDBexception& e){
        query_result = db.fetch(std::string("SELECT DISTINCT date FROM cards WHERE title = $1 and discount IS NOT NULL ORDER BY date ASC;"),  std::vector<std::string>{str});
    }
    if(query_result.empty()) throw EmptySampleProbabilityModelException("empty sample");
    if(query_result.size() < 3) throw SmallSampleForecastManagerException("small sample");

    auto first_date = db.fetch(std::string("SELECT date FROM cards WHERE title = $1 ORDER BY date ASC LIMIT 1;"), std::vector<std::string>{str});
    auto ymd = converte_string(first_date[0][0]);

    std::vector<int> sample;
    for(int i = 0; i < query_result.size();){
        if(converte_string(query_result[i][0]) == ymd){
            sample.push_back(1);
            i++;
        } else {
            sample.push_back(0);
        }
        std::chrono::sys_days date = std::chrono::sys_days{ymd};
        date += std::chrono::days{7};
        std::chrono::year_month_day n_ymd {date};
        ymd = n_ymd;
    }

    return sample;
}

int ForecastManager::get_probability(std::string str)
{
    auto load_cache = fc->get(str);
    if(load_cache != std::nullopt) return static_cast<int>(load_cache.value() * 100);

    std::vector<int> sample = build_dates(str);

    double prob;
    try{
        prob = get_better_probability(sample);
    } catch (NoSuitableProbabilityException& e){
        prob = 0.0;
    }

    fc->set(str, prob);
    return static_cast<int>(prob*100);
}

double ForecastManager::get_better_probability(const std::vector<int>& sample)
{
    std::vector<double> p,q;
    std::vector<int> new_sample;
    for(int i = 0; i <= 17 && i < sample.size(); ++i) new_sample.push_back(sample[i]);
    for(int i = 18; i < sample.size(); ++i){
        auto r = ms.select_best(new_sample);
        if(sample[i] == 1) p.push_back(r.best_probability);
        else q.push_back(r.best_probability);
        new_sample.push_back(sample[i]);
    }

    auto true_prob = ms.select_best(sample);

    double res_ra;

    try{
        res_ra = ra.roc_auc(p,q);
    } catch (EmptySampleROC_AUC_Exception& e){
        res_ra = 1.0;
    }

    return res_ra < 0.5 ? 1-true_prob.best_probability : true_prob.best_probability;
}

void ForecastManager::reset_cache()
{
    fc->reset();
}