project(simple_project)

set(BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(library)
add_subdirectory(application)
