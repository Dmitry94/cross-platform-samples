cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(hough-circles)

find_package(Qt5 REQUIRED Widgets)
find_package(OpenCV REQUIRED)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_cpp(MOC_WRAPPERS
    mainwindow.h
    worker-thread.h
    )

qt5_wrap_ui(UIC_WRAPPERS
    mainwindow.ui
    )

set(SRC_LIST
    convertions.h
    convertions.cpp
    worker-thread.h
    worker-thread.cpp
    mainwindow.h
    mainwindow.cpp
    main.cpp
    ${UIC_WRAPPERS}
    ${MOC_WRAPPERS})


add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(hough-circles ${OpenCV_LIBS} Qt5::Widgets)

install(TARGETS hough-circles RUNTIME DESTINATION bin)
