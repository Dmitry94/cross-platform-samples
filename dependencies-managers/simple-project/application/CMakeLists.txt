cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

project(application)
add_executable(application main.cpp)

find_path(LIBRARY_H_DIR library.h)
if(NOT LIBRARY_H_DIR)
	message(
		SEND_ERROR
		"Could not find library.h")
endif()

find_library(LIB library)
if(NOT LIB)
	message(
		SEND_ERROR
		"Could not find library")
endif()

add_library(library STATIC IMPORTED)

set_property(
	TARGET library
	PROPERTY INTERFACE_INCLUDE_DIRECTORIES
	${LIBRARY_H_DIR})

set_property(
	TARGET library
	PROPERTY IMPORTED_LOCATION
	${LIB})

target_link_libraries(application library)
