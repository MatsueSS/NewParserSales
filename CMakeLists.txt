cmake_minimum_required(VERSION 3.10)

project(NewParserSales)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    src/Reader.cpp
    src/Interface.cpp
)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES})

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})