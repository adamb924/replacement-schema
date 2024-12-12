cmake_minimum_required(VERSION 3.16)

project(replacer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(replacer
    main.cpp
	replacer.cpp
)

target_include_directories(replacer PUBLIC
    "."
    ${CMAKE_SOURCE_DIR}/replacement-schema
)

target_link_libraries(replacer
    PRIVATE
    replacement-schema
    Qt${QT_VERSION_MAJOR}::Core
)
