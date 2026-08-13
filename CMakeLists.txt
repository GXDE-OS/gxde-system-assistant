cmake_minimum_required(VERSION 3.7)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

project(sysbro)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sw_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif ()

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

    # generate qm files
    execute_process(COMMAND bash "translate_generation.sh"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif ()

# Sources files
file(GLOB SRCS "src/*.h" "src/*.cpp"
               "src/widgets/*.h" "src/widgets/*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Concurrent Gui Charts)
# 使用 dtk2widget（DTK2 API 的 Qt6 版本），而非 dtk6widget
PKG_SEARCH_MODULE(Dtk2Widget REQUIRED dtk2widget)
PKG_SEARCH_MODULE(Qt6Xdg REQUIRED Qt6Xdg)

# Tell CMake to create the executable
add_executable(sysbro ${SRCS} src/resources.qrc)

target_include_directories(sysbro PRIVATE
    ${Dtk2Widget_INCLUDE_DIRS}
    ${Qt6Xdg_INCLUDE_DIRS}
)
target_compile_options(sysbro PRIVATE ${Dtk2Widget_CFLAGS_OTHER})
target_link_directories(sysbro PRIVATE
    ${Dtk2Widget_LIBRARY_DIRS}
    ${Qt6Xdg_LIBRARY_DIRS}
)

target_link_libraries(sysbro
    ${Dtk2Widget_LIBRARIES}
    Qt6::Charts
    Qt6::Widgets
    Qt6::DBus
    Qt6::Concurrent
    ${Qt6Xdg_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX /usr)

add_subdirectory("sysbro-delete-files")
add_subdirectory("sysbro-service-mgr")
#add_subdirectory("sysbro-express")
add_subdirectory("sysbro-file-shredder")
add_subdirectory("sysbro-network-test")
add_subdirectory("sysbro-cpufreq")
add_subdirectory("sysbro-startup-apps")
add_subdirectory("gxde-sysbro-more-desktop")

# Install qm files
file(GLOB QM_FILES "src/translations/*.qm")
install(FILES ${QM_FILES}
        DESTINATION share/sysbro/translations)

# Install icon file
install(FILES src/resources/sysbro.svg DESTINATION share/icons/hicolor/scalable/apps)
#install(FILES src/resources/sysbro-express.svg DESTINATION share/icons/hicolor/scalable/apps)
install(FILES src/resources/sysbro-file-shredder.svg DESTINATION share/icons/hicolor/scalable/apps)
install(FILES src/resources/sysbro-network-test.svg DESTINATION share/icons/hicolor/scalable/apps)
install(FILES src/resources/sysbro-startup-apps.svg DESTINATION share/icons/hicolor/scalable/apps)

# Install policy file
install(FILES src/com.sysbro.pkexec.delete-files.policy DESTINATION share/polkit-1/actions)
install(FILES src/com.sysbro.pkexec.service.mgr.policy DESTINATION share/polkit-1/actions)

install(TARGETS sysbro DESTINATION bin)
install(FILES sysbro.desktop DESTINATION share/applications)
#install(FILES sysbro-express.desktop DESTINATION share/applications)
install(FILES sysbro-file-shredder.desktop DESTINATION share/applications)
install(FILES sysbro-network-test.desktop DESTINATION share/applications)
install(FILES sysbro-cpufreq.desktop DESTINATION share/applications)
install(FILES sysbro-startup-apps.desktop DESTINATION share/applications)

#install(FILES sysbro-express.desktop DESTINATION share/gxde/gxde-system-assistant/tool-extensions)
install(FILES sysbro-file-shredder.desktop DESTINATION share/GXDE/gxde-system-assistant/tool-extensions)
install(FILES sysbro-network-test.desktop DESTINATION share/GXDE/gxde-system-assistant/tool-extensions)
install(FILES sysbro-startup-apps.desktop DESTINATION share/GXDE/gxde-system-assistant/tool-extensions)
install(FILES sysbro-cpufreq.desktop DESTINATION share/GXDE/gxde-system-assistant/tool-extensions)

# Install GXDE file

install(FILES src/resources/gxde-system-assistant DESTINATION bin/)
