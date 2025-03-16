cmake_minimum_required(VERSION 3.6) 
project("Agument parser" VERSION 0.1 LANGUAGES CXX)

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(CMAKE_BUILD_TYPE "Release")
else()
	# add_compile_options(-g)
endif()
message("Build type used: ${CMAKE_BUILD_TYPE}")

add_compile_options(-Wall -std=gnu++23 -fno-stack-protector)

SET(
	LIB_UTIL
	"Util.cpp"
)

SET(
	LIB_PARSER
	"Parser.cpp"
)

add_library(Util			SHARED ${LIB_UTIL})
add_library(ArgumentParser	SHARED ${LIB_PARSER})

target_include_directories(Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ArgumentParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})