cmake_minimum_required(VERSION 3.1)

# https://github.com/libigl/libigl/issues/751
# http://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20160425/351643.html
if(APPLE)
  if(NOT CMAKE_LIBTOOL)
    find_program(CMAKE_LIBTOOL NAMES libtool)
  endif()
  if(CMAKE_LIBTOOL)
    set(CMAKE_LIBTOOL ${CMAKE_LIBTOOL} CACHE PATH "libtool executable")
    message(STATUS "Found libtool - ${CMAKE_LIBTOOL}")
    get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
    foreach(lang ${languages})
      # Added -c
      set(CMAKE_${lang}_CREATE_STATIC_LIBRARY
        "${CMAKE_LIBTOOL} -c -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")
    endforeach()
  endif()
endif()

### Available options ###
option(RTC_USE_STATIC_LIBRARY     "Use RTC as static library" OFF)
option(RTC_WITHOUT_COPYLEFT       "Disable Copyleft libraries"   OFF)
option(RTC_EXPORT_TARGETS         "Export RTC CMake targets"  OFF)



### Configuration
set(RTC_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(RTC_SOURCE_DIR "${RTC_ROOT}/src")#source code folder
set(RTC_EXTERNAL "${RTC_ROOT}/external")

# Dependencies are linked as INTERFACE targets unless libigl is compiled as a static library
if(RTC_USE_STATIC_LIBRARY)
  set(RTC_SCOPE PUBLIC)
else()
  set(RTC_SCOPE INTERFACE)
endif()

# Download and update 3rdparty libraries
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(RTCDownloadExternal)

# Provides igl_set_folders() to set folders for Visual Studio/Xcode
include(RTCFolders)

################################################################################
### rtc Common
################################################################################

add_library(rtc_common INTERFACE)
target_include_directories(rtc_common SYSTEM INTERFACE
  $<BUILD_INTERFACE:${RTC_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
# Export rtc_common as rtc::common
set_property(TARGET rtc_common PROPERTY EXPORT_NAME rtc::common)
if(RTC_USE_STATIC_LIBRARY)
  target_compile_definitions(rtc_common INTERFACE -DIGL_STATIC_LIBRARY)
endif()

# Transitive C++11 flags
include(CXXFeatures)
target_compile_features(rtc_common INTERFACE ${CXX11_FEATURES})

# Other compilation flags
if(MSVC)
  # Enable parallel compilation for Visual Studio
  target_compile_options(rtc_common INTERFACE /MP /bigobj)
  target_compile_definitions(rtc_common INTERFACE -DNOMINMAX)
endif()

# Controls whether to use the static MSVC runtime or not
include(RTCWindows)

if(BUILD_SHARED_LIBS)
  # Generate position independent code
  set_target_properties(rtc_common PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)
endif()

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

# Eigen
if(NOT TARGET Eigen3::Eigen)
  rtc_download_eigen()
  add_library(rtc_eigen INTERFACE)
  target_include_directories(rtc_eigen SYSTEM INTERFACE
    $<BUILD_INTERFACE:${RTC_EXTERNAL}/eigen>
    $<INSTALL_INTERFACE:include>
  )
  set_property(TARGET rtc_eigen PROPERTY EXPORT_NAME Eigen3::Eigen)
  add_library(Eigen3::Eigen ALIAS rtc_eigen)
endif()
add_library(rtc::common ALIAS rtc_common)

target_link_libraries(rtc_common INTERFACE Eigen3::Eigen)

# C++11 Thread library
find_package(Threads REQUIRED)
target_link_libraries(rtc_common INTERFACE ${CMAKE_THREAD_LIBS_INIT})


#######################################################################################################################



##glm

if(NOT TARGET glm::glm)
  rtc_download_glm()
  add_library(rtc_glm INTERFACE)
  target_include_directories(rtc_glm SYSTEM INTERFACE
          $<BUILD_INTERFACE:${RTC_EXTERNAL}/glm>
          $<INSTALL_INTERFACE:include>
          )
  set_property(TARGET rtc_glm PROPERTY EXPORT_NAME glm::glm)
  add_library(glm::glm ALIAS rtc_glm)
endif()

target_link_libraries(rtc_common INTERFACE glm::glm)
################################################################################

##opencv
#
#if(NOT TARGET glm::glm)todo
#  rtc_download_opencv()
#  add_library(rtc_opencv INTERFACE)
#  target_include_directories(rtc_opencv SYSTEM INTERFACE
#          $<BUILD_INTERFACE:${RTC_EXTERNAL}/glm>
#          $<INSTALL_INTERFACE:include>
#          )
#  set_property(TARGET rtc_glm PROPERTY EXPORT_NAME glm::glm)
#  add_library(glm::glm ALIAS rtc_glm)
#endif()
#
#target_link_libraries(rtc_common INTERFACE glm::glm)
#
#
#
#
#

################################################################################
set(DOCTEST_DIR "${RTC_EXTERNAL}/doctest")
if(NOT TARGET doctest)
  # call this "lib-cork" instead of "cork", otherwise cmake gets confused about
  # "cork" executable
  rtc_download_doctest()
  add_subdirectory("${DOCTEST_DIR}")
endif()
#target_include_directories(rtc_glm ${RTC_SCOPE} glm)
#target_include_directories(igl_cork ${IGL_SCOPE} "${CORK_DIR}/src")
#target_link_libraries(rtc_common INTERFACE doctest::doctest)


################################################################################

function(compile_rtc_module module_dir)
  string(REPLACE "/" "_" module_name "${module_dir}")
  if(module_name STREQUAL "core")
    set(module_libname "rtc")
  else()
    set(module_libname "rtc_${module_name}")
  endif()
  if(RTC_USE_STATIC_LIBRARY)
    file(GLOB SOURCES_RTC_${module_name}
      "${RTC_SOURCE_DIR}/rtc/${module_dir}/*.cpp"
      "${RTC_SOURCE_DIR}//${module_dir}/*.h*"
    )
    if(NOT RTC_WITHOUT_COPYLEFT)
      file(GLOB COPYLEFT_SOURCES_RTC_${module_name}
        "${RTC_SOURCE_DIR}/rtc/copyleft/${module_dir}/*.cpp"
        "${RTC_SOURCE_DIR}/rtc/copyleft/${module_dir}/*.h*"
      )
      list(APPEND SOURCES_RTC_${module_name} ${COPYLEFT_SOURCES_RTC_${module_name}})
    endif()
      message("going to add libary:" ${module_libname} STATIC ${SOURCES_RTC_${module_name}} ${ARGN})

    add_library(${module_libname} STATIC ${SOURCES_RTC_${module_name}} ${ARGN})
    if(MSVC)
      # Silencing some compile warnings
      target_compile_options(${module_libname} PRIVATE
        # Type conversion warnings. These can be fixed with some effort and possibly more verbose code.
        /wd4267 # conversion from 'size_t' to 'type', possible loss of data
        /wd4244 # conversion from 'type1' to 'type2', possible loss of data
        /wd4018 # signed/unsigned mismatch
        /wd4305 # truncation from 'double' to 'float'
        # This one is from template instantiations generated by autoexplicit.sh:
        /wd4667 # no function template defined that matches forced instantiation ()
        # This one is easy to fix, just need to switch to safe version of C functions
        /wd4996 # this function or variable may be unsafe
        # This one is when using bools in adjacency matrices
        /wd4804 #'+=': unsafe use of type 'bool' in operation
      )
    endif()
  else()
    add_library(${module_libname} INTERFACE)
  endif()

  target_link_libraries(${module_libname} ${RTC_SCOPE} rtc_common)
  if(NOT module_name STREQUAL "core")
    target_link_libraries(${module_libname} ${RTC_SCOPE} rtc)
  endif()

  # Alias target because it looks nicer
  message(STATUS "Creating target: rtc::${module_name} (${module_libname})")
  add_library(rtc::${module_name} ALIAS ${module_libname})
  # Export as rtc::${module_name}
    message("exporting: TARGET ${module_libname} PROPERTY EXPORT_NAME rtc::${module_name}")
  set_property(TARGET ${module_libname} PROPERTY EXPORT_NAME rtc::${module_name})
endfunction()

################################################################################
###  Core
################################################################################

if(RTC_USE_STATIC_LIBRARY)
  file(GLOB SOURCES_RTC
    "${RTC_SOURCE_DIR}/rtc/*.cpp"
    "${RTC_SOURCE_DIR}/rtc/*.h*"
    "${RTC_SOURCE_DIR}/rtc/copyleft/*.cpp"
    "${RTC_SOURCE_DIR}/rtc/copyleft/*.h*"
  )
endif()
message("source dir is : ${RTC_SOURCE_DIR}")
message("compiling files : ${SOURCES_RTC}")
compile_rtc_module("core" ${SOURCES_RTC})

################################################################################

#################################################################################

### Install and export all modules
message("RTC_EXPORT_TARGETS=${RTC_EXPORT_TARGETS}")
if(NOT RTC_EXPORT_TARGETS)
  message("dont export all modules")
  return()
endif()
message("exporting all rtc modules")

function(install_dir_files dir_name)
  if (dir_name STREQUAL "core")
    set(subpath "")
  else()
    set(subpath "/${dir_name}")
  endif()

  file(GLOB public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtc${subpath}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtc${subpath}/*.hpp
  )
    message(public headers to install are:${public_headers})
  set(files_to_install ${public_headers})

  if(NOT RTC_USE_STATIC_LIBRARY)
    file(GLOB public_sources
      ${CMAKE_CURRENT_SOURCE_DIR}/src/rtc${subpath}/*.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/rtc${subpath}/*.c
    )
  endif()
  list(APPEND files_to_install ${public_sources})

  install(
    FILES ${files_to_install}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtc${subpath}
  )
endfunction()

#################################################################################

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(TARGET rtc_eigen)
  set(RTC_EIGEN rtc_eigen)
else()
  set(RTC_EIGEN)
  message(WARNING "Trying to export RTC targets while using an imported target for Eigen.")
endif()

if(TARGET rtc_glm)
  set(RTC_GLM rtc_glm)
else()
  set(RTC_GLM)
  message(WARNING "Trying to export RTC targets while using an imported target for glm.")
endif()


# Install and export core library
install(
  TARGETS
    rtc
    rtc_common
    ${RTC_GLM}
    ${RTC_EIGEN}
  EXPORT rtc-export
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
export(
  TARGETS
    rtc
    rtc_common
    ${RTC_GLM}
    ${RTC_EIGEN}
  FILE rtc-export.cmake
)

# Install headers for core library
install_dir_files(core)
install_dir_files(copyleft)

# Write package configuration file
configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/RTC-config.cmake.in
  ${CMAKE_BINARY_DIR}/RTC-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/libigl/cmake
)
install(
  FILES
    ${CMAKE_BINARY_DIR}/rtc-config.cmake
  DESTINATION
    ${CMAKE_INSTALL_DATADIR}/rtc/cmake
)

# Write export file
export(EXPORT rtc-export
  FILE "${CMAKE_BINARY_DIR}/rtc-export.cmake"
)
install(EXPORT rtc-export DESTINATION ${CMAKE_INSTALL_DATADIR}/rtc/cmake FILE rtc-export.cmake)


export(PACKAGE rtc)

