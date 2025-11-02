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

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Boost 1.66 REQUIRED COMPONENTS system thread filesystem)
find_package(PkgConfig REQUIRED)
pkg_check_modules(vsomeip3 REQUIRED IMPORTED_TARGET vsomeip3)
include_directories(${vsomeip3_INCLUDE_DIRS})
link_directories(${vsomeip3_LIBRARY_DIRS})

# Sensor client
add_executable(sensor_client src/sensor_client.cc)
target_link_libraries(sensor_client
    PRIVATE
        Boost::system
        Boost::thread
        ${vsomeip3_LIBRARIES}
        pthread
)
target_compile_definitions(sensor_client PRIVATE APP_ENABLE_SIGNAL_HANDLING)

# Controller service
add_executable(controller_service src/controller_service.cc)
target_link_libraries(controller_service PRIVATE PkgConfig::vsomeip3)
target_compile_definitions(controller_service PRIVATE APP_ENABLE_SIGNAL_HANDLING)

# Dashboard service
add_executable(dashboard_service src/dashboard_service.cc)
target_link_libraries(dashboard_service PRIVATE PkgConfig::vsomeip3)
target_compile_definitions(dashboard_service PRIVATE APP_ENABLE_SIGNAL_HANDLING)
