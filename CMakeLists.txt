cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(GEODE_DISABLE_PRECOMPILED_HEADERS ON)

project(DemonsInBetween VERSION 1.4.12)

add_library(${PROJECT_NAME} SHARED
    src/classes/DIBInfoPopup.cpp
    src/classes/DIBSearchPopup.cpp
    src/classes/TableNode.cpp
    src/hooks/LevelBrowserLayer.cpp
    src/hooks/LevelCell.cpp
    src/hooks/LevelInfoLayer.cpp
    src/hooks/LevelPage.cpp
    src/hooks/LevelSearchLayer.cpp
    src/hooks/MenuLayer.cpp
    src/hooks/ProfilePage.cpp
    src/DemonsInBetween.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
