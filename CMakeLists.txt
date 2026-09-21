cmake_minimum_required(VERSION 3.10)

project(NewParserSales)

find_package(Threads REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/CatBoostStatic.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Wrappers/PostgresDB.cpp
    src/PyLoader.cpp
    #src/TelegramSender.cpp
    #src/JsonReader.cpp
    src/TelegramUser.cpp
    src/good_funcs.cpp
    src/BotTelegram.cpp
    src/Interface.cpp
    src/Forecast/Forecast.cpp
    src/Matrix.cpp
    src/FileMatcher.cpp
    src/ProductSearcher.cpp
    src/PrefixTree.cpp
    src/HiSquare.cpp
    src/ProductRecommendations.cpp
    src/PoolCards.cpp
    src/Product.cpp
    src/Parsers/Parser.cpp
    src/Parsers/ProductParser.cpp
    src/Parsers/PyAutoClickParser.cpp
    src/Parsers/PyHTMLParser.cpp
    src/Models/GeometricModel.cpp
    #src/Models/MarkovChain1Model.cpp
    #src/Models/MarkovChain2Model.cpp
    src/Models/ModelSelector.cpp
    src/Forecast/ForecastCache.cpp
    src/ROC_AUC.cpp
    src/Parsers/CppAutoClickParser.cpp
    src/Hypothesis.cpp
    src/IndependenceWeekHypothesis.cpp
    src/IndependenceSeasonHypothesis.cpp
    src/Wrappers/CurlWrapper.cpp
    src/Forecast/ForecastManager.cpp
    src/Wrappers/ModelWrapper.cpp
    src/Models/LogisticRegressionModel.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    CatBoost::Model
)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES})

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE linear)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/include)