cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(rss-client)

find_package(Qt5 REQUIRED Widgets Network Xml)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_cpp(MOC_WRAPPERS
    mainwindow.h
    )

qt5_wrap_ui(UIC_WRAPPERS
    mainwindow.ui
    )

set(SRC_LIST
    mainwindow.h
    mainwindow.cpp
    main.cpp
    ${UIC_WRAPPERS}
    ${MOC_WRAPPERS})


add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(rss-client Qt5::Widgets Qt5::Network Qt5::Xml)

install(TARGETS rss-client RUNTIME DESTINATION bin)
