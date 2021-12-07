cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 20)

project(advent-of-code-2021 CXX)

add_executable(day1_1 ${PROJECT_SOURCE_DIR}/src/day1_1.cpp)
add_executable(day1_2 ${PROJECT_SOURCE_DIR}/src/day1_2.cpp)

add_executable(day2_1 ${PROJECT_SOURCE_DIR}/src/day2_1.cpp)
add_executable(day2_2 ${PROJECT_SOURCE_DIR}/src/day2_2.cpp)

add_executable(day3_1 ${PROJECT_SOURCE_DIR}/src/day3_1.cpp)
add_executable(day3_2 ${PROJECT_SOURCE_DIR}/src/day3_2.cpp)

add_executable(day4_1 ${PROJECT_SOURCE_DIR}/src/day4_1.cpp)
add_executable(day4_2 ${PROJECT_SOURCE_DIR}/src/day4_2.cpp)

add_executable(day5_1 ${PROJECT_SOURCE_DIR}/src/day5_1.cpp)