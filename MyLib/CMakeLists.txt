cmake_minimum_required(VERSION 3.5)
project(MyLib VERSION 0.1 LANGUAGES CXX)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Test)
set(PROJECT_SOURCES
ctdataset.h
ctdataset.cpp
)
add_library(MyLib SHARED ${PROJECT_SOURCES})
add_definitions( -DMYLIB_EXPORT -DNOMINMAX )
target_link_libraries(MyLib PRIVATE Qt6::Widgets)
target_link_libraries(MyLib PRIVATE Qt6::Test)
