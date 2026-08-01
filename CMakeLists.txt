cmake_minimum_required(VERSION 3.16)
project(OzeillaVisualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(OzeillaVisualization
	src/main.cpp
	src/main_window.cpp
	src/translation.cpp
	include/main/main_window.h
	include/main/translation.h
	src/home_position.cpp
	include/home_position/home_position.h
)

target_include_directories(OzeillaVisualization PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(OzeillaVisualization PRIVATE Qt6::Widgets)