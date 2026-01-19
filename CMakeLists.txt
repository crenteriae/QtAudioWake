cmake_minimum_required(VERSION 3.16)
project(QtAudioWake VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(TONE_FREQUENCY "22000" CACHE STRING "Default tone frequency in Hz")
set(APP_NAME "QtAudioWake" CACHE STRING "Speaker keep-alive utility")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/components.cpp
    src/components.h
    src/tonegenerator.cpp
    src/tonegenerator.h
    src/settings.cpp
    src/settings.h
    src/constants.h
    resources.qrc
)

target_compile_definitions(QtAudioWake PRIVATE
    DEFAULT_TONE_FREQUENCY=${TONE_FREQUENCY}
    APPLICATION_NAME="${APP_NAME}")

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt6::Core
  Qt6::Widgets
  Qt6::Multimedia
)
