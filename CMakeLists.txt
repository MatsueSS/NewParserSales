cmake_minimum_required(VERSION 3.10)

project(NewParserSales)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/PostgresDB.cpp
    src/PyLoader.cpp
    src/TelegramSender.cpp
    src/JsonReader.cpp
    src/TelegramUser.cpp
    src/good_funcs.cpp
    src/BotTelegram.cpp
    src/Interface.cpp
    src/Forecast.cpp
    src/Matrix.cpp
    src/FileMatcher.cpp
    src/ProductSearcher.cpp
    src/PrefixTree.cpp
    src/HiSquare.cpp
    src/ProductRecommendations.cpp
    src/PoolCards.cpp
    src/Product.cpp
    src/Parser.cpp
    src/ProductParser.cpp
    src/PyAutoClickParser.cpp
    src/PyHTMLParser.cpp
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