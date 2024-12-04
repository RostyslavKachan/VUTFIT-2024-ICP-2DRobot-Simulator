cmake_minimum_required(VERSION 3.17)

project(Robots VERSION 0.1 LANGUAGES CXX)

# Set variables for finding Qt version 5.9.2 or newer, but with a minimum version of 5.9.2
set(CMAKE_PREFIX_PATH "/usr/local/share/Qt5.9.2/")
find_package(Qt5 5.9.2 COMPONENTS Widgets QUIET)

# If Qt 5.9.2 is not found, try to find Qt6
if(NOT Qt5_FOUND)
    find_package(Qt6 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION_STRING ${Qt6_VERSION_STRING})
    set(QT_LIBRARIES ${Qt6Widgets_LIBRARIES})
    set(QT_INCLUDE_DIRS ${Qt6Widgets_INCLUDE_DIRS})
else()
    set(QT_VERSION_STRING ${Qt5_VERSION_STRING})
    set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})
    set(QT_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIRS})
endif()

# Settings for automatic UIC, MOC, and RCC generation
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Setting the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project source files
set(PROJECT_SOURCES
        main.cpp
        customgraphicsscene.h customgraphicsscene.cpp
        obstacle.h obstacle.cpp
        environment.h environment.cpp
        robot.h robot.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        welcomewidget.h welcomewidget.cpp welcomewidget.ui
        simulationwidget.h simulationwidget.cpp simulationwidget.ui
        creatorwidget.h creatorwidget.cpp creatorwidget.ui
        objectpainter.h objectpainter.cpp
        mappainter.h mappainter.cpp
)

# Prepare target for Qt 5 or Qt 6
add_executable(Robots ${PROJECT_SOURCES})

# Linking libraries
target_link_libraries(Robots PRIVATE ${QT_LIBRARIES})

# Setting target properties
set_target_properties(Robots PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Installation
include(GNUInstallDirs)
install(TARGETS Robots
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
