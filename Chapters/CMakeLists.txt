cmake_minimum_required(VERSION 3.1)
project(rtc_chapters)

### Conditionally compile certain modules depending on libraries found on the system
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)
message(compiling rtc_chapters  directory)
message(CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH})

### Adding libIGL: choose the path to your local copy libIGL
if(NOT TARGET rtc_common)
    include(RTC)
else()
    include(RTCDownloadExternal)
endif()

### Download data
#rtc_download_tutorial_data()

### Output directories
if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

### Choose which chapters to compile
option(TUTORIALS_CHAPTER1 "Compile chapter 1" ON)
option(TUTORIALS_CHAPTER2 "Compile chapter 2" ON)
option(TUTORIALS_CHAPTER4 "Compile chapter 4" ON)
option(TUTORIALS_CHAPTER5 "Compile chapter 5" ON)
option(TUTORIALS_CHAPTER6 "Compile chapter 6" ON)
option(TUTORIALS_CHAPTER7 "Compile chapter 7" ON)



# Store location of tutorial/shared directory
set(TUTORIAL_SHARED_PATH ${RTC_EXTERNAL}/../tutorial/data CACHE PATH "location of shared tutorial resources")
add_library(tutorials INTERFACE)
target_compile_definitions(tutorials INTERFACE "-DTUTORIAL_SHARED_PATH=\"${TUTORIAL_SHARED_PATH}\"")
target_include_directories(tutorials INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Chapter 1
if(TUTORIALS_CHAPTER1)
    add_subdirectory("Chapter1")
endif()
if(TUTORIALS_CHAPTER2)
    add_subdirectory("Chapter2")
endif()
if(TUTORIALS_CHAPTER4)
    add_subdirectory(Chapter4)
endif()

if(TUTORIALS_CHAPTER5)
    add_subdirectory(Chapter5)
endif()

if(TUTORIALS_CHAPTER6)
    add_subdirectory(Chapter6)
endif()
if(TUTORIALS_CHAPTER7)
    add_subdirectory(Chapter7)
endif()