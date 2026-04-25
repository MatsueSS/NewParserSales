cmake_minimum_required(VERSION 3.10)

project(tests)

# include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    test_main.cpp
    test_forecast.cpp
    test_forecast_cache.cpp
    test_model_probability.cpp
    test_prefix_tree.cpp
    test_pool_cards.cpp
    test_hi_square.cpp
    test_roc_auc.cpp
    test_postgres_db.cpp
    test_curl_wrapper.cpp

    ../src/good_funcs.cpp
    ../src/PostgresDB.cpp
    ../src/CurlWrapper.cpp
    ../src/HiSquare.cpp
    
    ../src/Forecast.cpp
    ../src/ForecastCache.cpp
    ../src/ModelSelector.cpp
    ../src/MarkovChain1Model.cpp
    ../src/MarkovChain2Model.cpp
    ../src/GeometricModel.cpp

    ../src/PrefixTree.cpp
    ../src/PoolCards.cpp
    ../src/Product.cpp

    ../src/HiSquare.cpp
    ../src/Hypothesis.cpp
    ../src/IndependenceWeekHypothesis.cpp
    ../src/IndependenceSeasonHypothesis.cpp
    ../src/ROC_AUC.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/../include
)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES})

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})


target_compile_options(tests PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g)
target_link_options(tests PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak)