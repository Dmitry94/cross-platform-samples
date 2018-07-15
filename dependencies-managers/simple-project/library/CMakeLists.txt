cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

project(library)
add_library(library library.cpp library.h)

get_property(LIB_TYPE TARGET library PROPERTY TYPE)

if(LIB_TYPE STREQUAL SHARED_LIBRARY)
	install(
		TARGETS library
		COMPONENT user
		RUNTIME
			DESTINATION bin
		LIBRARY
			DESTIBATION Lib)
endif()

install(
	TARGETS library
	COMPONENT developer
	RUNTIME
		DESTINATION bin
	LIBRARY
		DESTINATION lib
	ARCHIVE
		DESTINATION lib)

install(
	DIRECTORY .
	DESTINATION include
	COMPONENT developer
	FILES_MATCHING
		PATTERN "*.h")

target_include_directories(library INTERFACE)
