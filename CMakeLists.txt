cmake_minimum_required(VERSION 3.6) 
project("Argument parser" VERSION 1.2 LANGUAGES CXX)

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(CMAKE_BUILD_TYPE "Release")
else()
	add_compile_options(-g)
endif()
message("Build type used: ${CMAKE_BUILD_TYPE}")

add_compile_options(-Wall -std=gnu++23)

set(
	LIB_UTIL
	"Util.cpp"
)

set(
	LIB_PARSER
	"Parser.cpp"
)

add_library(Util			SHARED ${LIB_UTIL})
add_library(ArgumentParser	SHARED ${LIB_PARSER})
