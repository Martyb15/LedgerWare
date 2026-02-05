cmake_minimum_required(VERSION 3.20)
project(LedgerWare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ledgerware
    src/main.cpp
)

find_package(fmt CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

target_link_libraries(ledgerware PRIVATE
    fmt::fmt
    unofficial::sqlite3::sqlite3
)

target_include_directories(ledgerware PRIVATE include)
