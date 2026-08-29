#include "Models/LogisticRegressionModel.h"

#include "Features_regression/FeaturePattern00.h"
#include "Features_regression/FeatureExtractor.h"
#include "Features_regression/FeatureKurtosis.h"
#include "Features_regression/FeatureAutocorrLag2.h"
#include "Features_regression/FeatureMinRun.h"
#include "Features_regression/FeatureMaxRun.h"
#include "Features_regression/FeatureTransitions01.h"
#include "Features_regression/FeatureTransitions10.h"
#include "Features_regression/FeatureSum.h"
#include "Features_regression/FeatureWeightSum.h"
#include "Features_regression/FeatureMode.h"
#include "Features_regression/FeatureEntropy.h"
#include "Features_regression/FeatureLag1.h"
#include "Features_regression/FeatureLag2.h"

#include <iostream>

LogisticRegressionModel::LogisticRegressionModel()
{
    name = TypeModel::LOGISTIC_REGRESSION;
}

void LogisticRegressionModel::change_regular(double C) noexcept
{
    mw.change_C(C);
}

double LogisticRegressionModel::calculate_bic(std::vector<std::vector<double>>&& X)
{
    int n = X.size();
    int k = mw.count_signs;
    
    double log_likelihood = 0.0;
    const double epsilon = 1e-15;
    
    for (int i = 0; i < n; i++) {
        std::vector<feature_node> nodes(k + 1);
        for (int j = 0; j < k; j++) {
            nodes[j].index = j + 1;
            nodes[j].value = X[i][j];
        }
        nodes[k].index = -1;
        
        double probs[2];
        predict_probability(mw.mw_ptr.get(), nodes.data(), probs);
        
        double prob = (mw.sample[i] == 1) ? probs[1] : probs[0];
        prob = std::max(epsilon, std::min(1.0 - epsilon, prob));
        
        log_likelihood += log(prob);
    }
    
    return -2.0 * log_likelihood + k * log(n);
}

std::vector<int> LogisticRegressionModel::find_tuple_features(int n, const std::vector<result_normalize>& features) const
{
    if(n == 1){
        std::pair<double, int> result = {0, -1};
        for(int i = 0; i < features.size(); ++i){
            double corr = Feature<FeatureAutocorrLag2>::pearson_correlation(features[i].norm_sample, mw.sample);
            double ncorr = std::fabs(corr);
            if(result.first < ncorr){
                result.first = ncorr;
                result.second = i;
            }
        }

        return {result.second};
    }

    std::vector<std::tuple<double, int, int>> correlations;
    for(int i = 0; i < features.size(); ++i){
        for(int j = i+1; j < features.size(); ++j){
            double corr = Feature<FeatureAutocorrLag2>::pearson_correlation(features[i].norm_sample, features[j].norm_sample);
            correlations.push_back(std::make_tuple(corr, i, j));
        }
    }

    sort(correlations.begin(), correlations.end(), std::greater<>());

    std::vector<bool> removed(features.size(), false);
    for(auto& [corr, i, j] : correlations){
        if(corr > 0.7 && !removed[i] && !removed[j]) removed[j] = true;
    }   

    std::vector<int> selected;
    for(int i = 0; i < features.size() && selected.size() < n; ++i){
        if(!removed[i]) selected.push_back(i);
    }
    return selected;
}

double LogisticRegressionModel::make_train(const std::vector<int>& sample)
{
    using MyExtractor = FeatureExtractor<FeaturePattern00, FeatureKurtosis, FeatureAutocorrLag2, FeatureMinRun,
                        FeatureMaxRun, FeatureLag1, FeatureLag2, FeatureTransitions01, FeatureTransitions10,
                        FeatureSum, FeatureWeightSum, FeatureMode, FeatureEntropy>;

    std::vector<double> pattern00_raw, min_run_raw, max_run_raw, kurtosis_raw, autocorr_raw, lag1, lag2,
                    trans01_raw, trans10_raw, sum_raw, w_sum_raw, mode_raw, entropy_raw;

    std::vector<double> y;

    int window_size = 12;

    for (size_t i = window_size; i < sample.size(); i++) {
        lag1.push_back(sample[i-1]);
        lag2.push_back(sample[i-2]);

        std::vector<int> window;
        for(size_t j = i - window_size; j < i; ++j){
            window.push_back(sample[j]);
        }

        std::vector<double> ext_result = MyExtractor::extract(window);
        
        pattern00_raw.push_back(ext_result[0]);
        kurtosis_raw.push_back(ext_result[1]);
        autocorr_raw.push_back(ext_result[2]);
        min_run_raw.push_back(ext_result[3]);
        max_run_raw.push_back(ext_result[4]);
        trans01_raw.push_back(ext_result[5]);
        trans10_raw.push_back(ext_result[6]);
        sum_raw.push_back(ext_result[7]);
        w_sum_raw.push_back(ext_result[8]);
        mode_raw.push_back(ext_result[9]);
        entropy_raw.push_back(ext_result[10]);

        y.push_back(sample[i]);
    }   

    mw.sample = y;

    int n = y.size();
    
    int train_size = n * 0.5;

    std::vector<int> back_sample;
    for(int i = sample.size()-window_size; i < sample.size(); ++i){
        back_sample.push_back(sample[i]);
    }
    std::vector<result_normalize> norm_result = MyExtractor::normalize({pattern00_raw, kurtosis_raw, autocorr_raw,
            lag1, lag2, min_run_raw, max_run_raw, trans01_raw, trans10_raw, sum_raw, w_sum_raw, mode_raw, entropy_raw}, 
            train_size, back_sample);

    int k = 1;

    std::vector<int> fea0;
    std::vector<int> fea1;

    double bic1 = -1, bic2 = 0;

    do{
        fea0 = find_tuple_features(k++, norm_result);
        fea1 = find_tuple_features(k, norm_result);

        std::vector<result_normalize> norm1, norm2;
        std::vector<std::vector<double>> for_bic1 (train_size), for_bic2 (train_size);

        for(int i : fea0) norm1.push_back(norm_result[i]);
        for(int i : fea1) norm2.push_back(norm_result[i]);

        int d;

        for(int i = 0; i < train_size; ++i){
            for(int j : fea0) for_bic1[i].push_back(norm_result[j].norm_sample[i]);
            for(int j : fea1) for_bic2[i].push_back(norm_result[j].norm_sample[i]);
        }   

        mw.change_C(0.1);
        mw.set_signs(std::move(norm2), y, train_size);
        mw.train_model();
        bic2 = calculate_bic(std::move(for_bic2));

        mw.change_C(0.1);
        mw.set_signs(std::move(norm1), y, train_size);
        mw.train_model();
        bic1 = calculate_bic(std::move(for_bic1));

        std::cout << bic1 << '\n';
        for(int j : fea0) std::cout << j << ' ';
        std::cout << '\n';

        std::cout << bic2 << '\n';
        for(int j : fea1) std::cout << j << ' ';
        std::cout << '\n';
        
    } while(bic2 < bic1);

    train_size = n;

    double bic;
    std::vector<result_normalize> total_norm;
    std::vector<std::vector<double>> total_bic (train_size);
    for(int i : fea0) total_norm.push_back(norm_result[i]);
    for(int i = 0; i < train_size; ++i){
        for(int j : fea0) total_bic[i].push_back(norm_result[j].norm_sample[i]);
    }

    mw.change_C(0.1);
    mw.set_signs(std::move(total_norm), y, train_size);
    mw.train_model();
    bic = calculate_bic(std::move(total_bic));

    return bic;
}

std::pair<double, double> LogisticRegressionModel::calculate_bic_with_prob(const std::vector<int>& sample)
{
    double b = make_train(sample);
    return {mw.get_probability(), b};
}