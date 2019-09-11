cmake_minimum_required(VERSION 3.12)

project(metaf)

set(CMAKE_CXX_STANDARD 17)
enable_language(CXX)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wno-c++11-extensions")
set(CMAKE_CXX_COMPILER emcc)

if(CMAKE_CXX_COMPILER MATCHES emcc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s WASM=1")
endif()

include_directories(${PROJECT_SOURCE_DIR}/src)

set(GOOGLETEST_DIR ${PROJECT_SOURCE_DIR}/googletest/googletest)



# Sphinx documentation

find_program(SPHINX_BUILD NAMES sphinx-build)

if (SPHINX_BUILD) 
	set(SPHINX_DOCTREE_DIR "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/sphinx")
	set(SPHINX_SOURCE_DIR "${PROJECT_SOURCE_DIR}/docs/sphinx")
	set(SPHINX_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/docs")
	add_custom_command(OUTPUT sphinx-docs 
		COMMAND ${SPHINX_BUILD} -d ${SPHINX_DOCTREE_DIR} ${SPHINX_SOURCE_DIR} ${SPHINX_OUTPUT_DIR})
	add_custom_target(docs DEPENDS sphinx-docs)
else()
	message ("Sphinx (https://www.sphinx-doc.org) not found, skipping documentation building")
endif()


# Example: Explain METAR/TAF in English language

add_executable(example_explain examples/explain.cpp)

set_target_properties(example_explain PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/examples
	OUTPUT_NAME "explain.html"
	LINK_FLAGS "-Oz --shell-file ${PROJECT_SOURCE_DIR}/examples/shellfiles/explain_shell.html -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']\""
)


# Example: Weather summary

add_executable(example_summary examples/summary.cpp)

set_target_properties(example_summary PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/examples
	OUTPUT_NAME "summary.html"
	LINK_FLAGS "-Oz --bind --shell-file ${PROJECT_SOURCE_DIR}/examples/shellfiles/summary_shell.html"
)


# Example: Tutorial

add_executable(example_tutorial examples/tutorial.cpp)

set_target_properties(example_tutorial PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/examples
	OUTPUT_NAME "tutorial.html"
	LINK_FLAGS "-Oz"
)


# All examples

add_custom_target(examples ALL DEPENDS example_tutorial example_explain example_summary)



# Performance check

add_executable(performance 
	${PROJECT_SOURCE_DIR}/performance/main.cpp 
	${PROJECT_SOURCE_DIR}/test/testdata_real.cpp
)

set_target_properties(performance PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/performance
	OUTPUT_NAME "check.html"
)

target_include_directories(performance PRIVATE 
	${PROJECT_SOURCE_DIR}/test
)



# Automated tests

file(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(tests 
	${TEST_SRC} 
	${GOOGLETEST_DIR}/src/gtest-all.cc
)

target_include_directories(tests PRIVATE 
	${PROJECT_SOURCE_DIR}/test
	${GOOGLETEST_DIR}
	${GOOGLETEST_DIR}/include
)

set_target_properties(tests PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/test
	OUTPUT_NAME "main.html"
	LINK_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0 --emrun"
)

target_compile_options(tests PRIVATE
	-s DISABLE_EXCEPTION_CATCHING=0 
)

#Copy assets for webpages

add_custom_target(copy_assets ALL)

add_custom_command(TARGET copy_assets 
	PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E 
    copy_directory ${CMAKE_SOURCE_DIR}/pages/ ${CMAKE_CURRENT_BINARY_DIR}/bin)
