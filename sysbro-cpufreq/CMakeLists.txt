cmake_minimum_required(VERSION 3.7)

project(sysbro-cpufreq)

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
add_executable(sysbro-cpufreq ${SRCS} resoureces.qrc)

target_include_directories(sysbro-cpufreq PRIVATE ${Dtk2Widget_INCLUDE_DIRS})
target_compile_options(sysbro-cpufreq PRIVATE ${Dtk2Widget_CFLAGS_OTHER})
target_link_directories(sysbro-cpufreq PRIVATE ${Dtk2Widget_LIBRARY_DIRS})

target_link_libraries(sysbro-cpufreq
    ${Dtk2Widget_LIBRARIES}
    Qt6::Widgets
    Qt6::DBus
    Qt6::Concurrent
    Qt6::Network
)

file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES}
        DESTINATION share/sysbro-cpufreq/translations)


install(TARGETS sysbro-cpufreq DESTINATION bin)

