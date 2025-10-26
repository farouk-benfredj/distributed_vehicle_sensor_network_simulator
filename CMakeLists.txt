cmake_minimum_required(VERSION 3.22)

project(distributed_vehicle_sensor_network_simulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER g++-13)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(vsomeip3 REQUIRED)

# sensor_service
add_executable(sensor_service src/sensor_service.cc)
target_link_libraries(sensor_service PRIVATE vsomeip3)

# controller_service
add_executable(controller_service src/controller_service.cc)
target_link_libraries(controller_service PRIVATE vsomeip3)

# dashboard_service
add_executable(dashboard_service src/dashboard_service.cc)
target_link_libraries(dashboard_service PRIVATE vsomeip3)
