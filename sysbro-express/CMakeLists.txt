cmake_minimum_required(VERSION 3.7)

project(sysbro-express)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Sources files
file(GLOB SRCS "*.h" "*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Concurrent Gui Network)
# 使用 dtk2widget（DTK2 API 的 Qt6 版本），而非 dtk6widget
PKG_SEARCH_MODULE(Dtk2Widget REQUIRED dtk2widget)

# Tell CMake to create the executable
add_executable(sysbro-express ${SRCS} resources.qrc)

target_include_directories(sysbro-express PRIVATE ${Dtk2Widget_INCLUDE_DIRS})
target_compile_options(sysbro-express PRIVATE ${Dtk2Widget_CFLAGS_OTHER})
target_link_directories(sysbro-express PRIVATE ${Dtk2Widget_LIBRARY_DIRS})

target_link_libraries(sysbro-express
    ${Dtk2Widget_LIBRARIES}
    Qt6::Widgets
    Qt6::DBus
    Qt6::Concurrent
    Qt6::Network
)

install(TARGETS sysbro-express DESTINATION bin)


