cmake_minimum_required(VERSION 3.7)

project(sysbro-delete-files)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(sysbro-delete-files "main.cpp")
target_link_libraries(sysbro-delete-files Qt6::Core)
install(TARGETS sysbro-delete-files DESTINATION bin)
