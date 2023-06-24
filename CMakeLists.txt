cmake_minimum_required(VERSION 3.10)

project(compiler LANGUAGES CXX)

file(GLOB SOURCES "lexical_analysis/src/*.cpp" "grammar_analysis/src/*.cpp")

add_executable(main main.cpp ${SOURCES})

target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/lexical_analysis/include ${PROJECT_SOURCE_DIR}/grammar_analysis/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})