cmake_minimum_required(VERSION 3.5)

project(RecipeParser VERSION 0.1 LANGUAGES CXX)

add_subdirectory(dlls)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)

set(PROJECT_SOURCES
        parser.cpp
        parser.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RecipeParser
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(RecipeParser SHARED
            ${PROJECT_SOURCES}
        )

    else()
        add_executable(RecipeParser
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(RecipeParser PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RecipeParser PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(RecipeParser PRIVATE Qt${QT_VERSION_MAJOR}::Sql ${CMAKE_DL_LIBS})

set_target_properties(RecipeParser PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RecipeParser)
endif()


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#install(TARGETS RecipeParser DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#install(TARGETS eda RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/dll)

install(TARGETS RecipeParser DESTINATION .)
install(TARGETS eda RUNTIME DESTINATION ./dll)
install(FILES ./URList.txt
./Recipes.db
DESTINATION .) 

