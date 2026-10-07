cmake_minimum_required(VERSION 3.21)
project(VectorForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
set(LIB_SOURCES ${SOURCES})
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_executable(VectorForge ${SOURCES})
target_link_libraries(VectorForge PRIVATE Qt6::Widgets)

add_executable(Benchmark tests/benchmark.cpp ${LIB_SOURCES})
target_include_directories(Benchmark PRIVATE src)
target_link_libraries(Benchmark PRIVATE Qt6::Widgets)