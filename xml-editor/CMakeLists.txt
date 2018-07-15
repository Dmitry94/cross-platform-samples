cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(xml-editor)

find_package(Qt5 REQUIRED Widgets LinguistTools Xml)

include_directories(gui/)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_cpp(MOC_WRAPPERS
    gui/window.h
    )

qt5_wrap_ui(UIC_WRAPPERS
    gui/window.ui
    )

set(SRC_LIST
    gui/window.h
    gui/window.cpp
    main.cpp
    ${UIC_WRAPPERS}
    ${MOC_WRAPPERS})


set(FILES_TO_TRANSLATE ${SRC_LIST})

set(TS_FILES translations/lang_ru_RU.ts translations/lang_en_US.ts)

option(UPDATE_TRANSLATIONS "Update source translation (files translations/*.ts)." ON)

if(UPDATE_TRANSLATIONS)
    message(STATUS "UPDATE_TRANSLATIONS option is set.")
    qt5_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

add_custom_target(translations DEPENDS ${QM_FILES})
set_property(DIRECTORY PROPERTY CLEAN_NO_CUSTOM TRUE)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${QRC_WRAPPERS})

target_link_libraries(xml-editor Qt5::Widgets Qt5::Xml)

install(TARGETS xml-editor RUNTIME DESTINATION bin)
install(FILES ${QM_FILES} DESTINATION translations)

get_property(LIB_QT5CORE_PATH TARGET Qt5::Core PROPERTY IMPORTED_LOCATION_RELEASE)
get_filename_component(QT_LIBRARY_DIR "${LIB_QT5CORE_PATH}" DIRECTORY)
set(EXE_PATH bin/xml-editor${CMAKE_EXECUTABLE_SUFFIX})
include(DeployQt4)
install_qt4_executable(
        "${EXE_PATH}" # executable file
        "" # modules
        "" # libraries
        "${QT_LIBRARY_DIR}") # directories for libraries search

