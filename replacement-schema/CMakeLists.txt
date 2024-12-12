cmake_minimum_required(VERSION 3.16)

project(replacement-schema LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_POSTFIX d)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

add_library(replacement-schema SHARED
    replacement.cpp 
    replacementgroup.cpp 
    replacementmodel.cpp 
    replacementpass.cpp 
    replacementreport.cpp 
    replacementreportmodel.cpp 
    replacementschema.cpp
)

target_link_libraries(replacement-schema PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
 )

target_compile_definitions(replacement-schema PRIVATE REPLACEMENTSCHEMA_LIBRARY)

install(TARGETS replacement-schema
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

# this installs some non-public headers, but so be it
install(DIRECTORY "${CMAKE_SOURCE_DIR}/replacement-schema"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING
        PATTERN "*.h"
)
