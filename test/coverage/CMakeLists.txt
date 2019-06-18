cmake_minimum_required(VERSION 3.10)

project(metaf-test-coverage)

set(CMAKE_CXX_STANDARD 17)
enable_language(CXX)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -std=c++17 -stdlib=libc++ -O0 -g --coverage")
set(CMAKE_CXX_COMPILER clang++)

include_directories(${PROJECT_SOURCE_DIR}/../../src)

set(GOOGLETEST_DIR ${PROJECT_SOURCE_DIR}/../../googletest/googletest)

file(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/../../test/*.cpp)

add_executable(testcov 
	${TEST_SRC} 
	${GOOGLETEST_DIR}/src/gtest-all.cc
)

target_include_directories(testcov PRIVATE 
	${PROJECT_SOURCE_DIR}/../../test
	${GOOGLETEST_DIR}
	${GOOGLETEST_DIR}/include
)

set_target_properties(testcov PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../bin/test
	OUTPUT_NAME "testcov"
	LINK_FLAGS "-lpthread -lm --coverage"
)
