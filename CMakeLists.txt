cmake_minimum_required(VERSION 3.0)

set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=17")

if(NOT TARGET ftest)

project(ftest)

add_library(ftest test_logging.cpp)

install(TARGETS ftest LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/" #source directory
        DESTINATION "include/ftest" #target directory
        FILES_MATCHING #install only matched files
        PATTERN "*.hpp" #select header files
        PATTERN "build" EXCLUDE
        PATTERN ".*" EXCLUDE
        PATTERN "**CMakeFiles**" EXCLUDE)

endif()
