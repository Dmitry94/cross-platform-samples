cmake_minimum_required(VERSION 3.2)
                                                    
set (CMAKE_CXX_STANDARD 11)

project(clocks)

find_package(Qt5 REQUIRED Core Gui Qml Quick)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_add_resources(
    QRC_WRAPPERS
    clocks.qrc)


qt5_wrap_cpp(MOC_WRAPPERS
    time-trigger.h
    )


set(SRC_LIST
    clocks.qml
    main.cpp
    time-trigger.h
    time-trigger.cpp
    ${MOC_WRAPPERS}
    ${QRC_WRAPPERS})



add_executable(${PROJECT_NAME} ${SRC_LIST} )

target_link_libraries(${PROJECT_NAME}  Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
