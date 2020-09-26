cmake_minimum_required(VERSION 3.1)


project(ray_tracer)

# Detects whether this is a top-level project
get_directory_property(RTC_PARENT_DIR PARENT_DIRECTORY)
message(RTC_PARENT_DIR=${RTC_PARENT_DIR})
if(NOT RTC_PARENT_DIR)
    set(RTC_TOPLEVEL_PROJECT ON)
else()
    set(RTC_TOPLEVEL_PROJECT OFF)
endif()

message(RTC_TOPLEVEL_PROJECT=${RTC_TOPLEVEL_PROJECT})

# Build tests and tutorials
option(RTC_BUILD_TESTS      "Build RTC tests"        ${RTC_TOPLEVEL_PROJECT})
option(RTC_BUILD_CHAPTERS  "Build CHAPTERS"         ${RTC_TOPLEVEL_PROJECT})
option(RTC_EXPORT_TARGETS   "Export  CMake targets"   ${RTC_TOPLEVEL_PROJECT})
set(RTC_EXPORT_TARGETS ON)
set(RTC_BUILD_CHAPTERS ON)
set(RTC_USE_STATIC_LIBRARY ON)
message(RTC_EXPORT_TARGETS=${RTC_EXPORT_TARGETS})
# (by default, static build is off since libigl is a header-only library)

option(RTC_USE_STATIC_LIBRARY "Use  as static library" OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

### conditionally compile certain modules depending on libraries found on the system
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

### Adding libIGL: choose the path to your local copy libIGL
include(RTC)

add_executable(${PROJECT_NAME} src/rtc/main.cpp src/rtc/core/material.h)
target_link_libraries(${PROJECT_NAME} rtc::core doctest::doctest)

message("dasodasdasplkdsk=,RTC_BUILD_CHAPTERS= ${RTC_BUILD_CHAPTERS}")
if(RTC_BUILD_CHAPTERS)
    message("compiling rtc chapters")
    add_subdirectory(Chapters)
endif()
#
if(RTC_BUILD_TESTS)
#    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
#
#if(RTC_TOPLEVEL_PROJECT)
#    # Set folders for Visual Studio/Xcode
#    igl_set_folders()
#endif()
