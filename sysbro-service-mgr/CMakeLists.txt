cmake_minimum_required(VERSION 3.7)

project(sysbro-service-mgr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(sysbro-service-mgr "main.cpp")
target_link_libraries(sysbro-service-mgr Qt6::Core)
install(TARGETS sysbro-service-mgr DESTINATION bin)
