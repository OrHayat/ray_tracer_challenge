################################################################################
include(DownloadProject)

# With CMake 3.8 and above, we can hide warnings about git being in a
# detached head by passing an extra GIT_CONFIG option.
set(RTC_EXTRA_OPTIONS TLS_VERIFY OFF)
if(NOT (${CMAKE_VERSION} VERSION_LESS "3.8.0"))
	list(APPEND RTC_EXTRA_OPTIONS GIT_CONFIG advice.detachedHead=false)
endif()

# On CMake 3.6.3 and above, there is an option to use shallow clones of git repositories.
# The shallow clone option only works with real tags, not SHA1, so we use a separate option.
set(RTC_BRANCH_OPTIONS)
if(NOT (${CMAKE_VERSION} VERSION_LESS "3.6.3"))
	# Disabled for now until we can make sure that it has no adverse effects
	# (Downside is that the eigen mirror is huge again)
	# list(APPEND LIBIGL_BRANCH_OPTIONS GIT_SHALLOW 1)
endif()

option(RTC_SKIP_DOWNLOAD "Skip downloading external libraries" OFF)

# Shortcut functions
function(rtc_download_project_aux name source)
	if(NOT RTC_SKIP_DOWNLOAD)
		download_project(
			PROJ         ${name}
			SOURCE_DIR   "${source}"
			DOWNLOAD_DIR "${RTC_EXTERNAL}/.cache/${name}"
			QUIET
			${LIBIGL_EXTRA_OPTIONS}
			${ARGN}
		)
	endif()
endfunction()

function(rtc_download_project name)
	rtc_download_project_aux(${name} "${RTC_EXTERNAL}/${name}" ${ARGN})
endfunction()

################################################################################


## Eigen
set(LIBIGL_EIGEN_VERSION 3.3.7 CACHE STRING "Default version of Eigen used by libigl.")
function(rtc_download_eigen)
	rtc_download_project(eigen
		GIT_REPOSITORY https://github.com/eigenteam/eigen-git-mirror.git
		GIT_TAG        ${LIBIGL_EIGEN_VERSION}
		${LIBIGL_BRANCH_OPTIONS}
	)
endfunction()

## glm
function(rtc_download_glm)
	rtc_download_project(glm
			GIT_REPOSITORY https://github.com/g-truc/glm.git
			GIT_TAG       0.9.9.8
			)
endfunction()

##opencv

function(rtc_download_opencv)
	rtc_download_project(opencv
			GIT_REPOSITORY https://github.com/opencv/opencv.git
			GIT_TAG      4.4.0
			)
endfunction()



## doctest
function(rtc_download_doctest)
	rtc_download_project(doctest
		GIT_REPOSITORY https://github.com/onqtam/doctest.git
		GIT_TAG        2.4.0
	)
endfunction()



################################################################################
#
### Test data
#function(igl_download_test_data)
#	igl_download_project_aux(test_data
#		"${LIBIGL_EXTERNAL}/../tests/data"
#		GIT_REPOSITORY https://github.com/libigl/libigl-tests-data
#		GIT_TAG        b5dddf45e329af685cd107e38770a28cfc18eb15
#	)
#endfunction()
#
### Tutorial data
#function(igl_download_tutorial_data)
#	igl_download_project_aux(tutorial_data
#		"${LIBIGL_EXTERNAL}/../tutorial/data"
#		GIT_REPOSITORY https://github.com/libigl/libigl-tutorial-data
#		GIT_TAG        fb5fa00bc4ede64b36002d703ce541552370b6e9
#	)
#endfunction()
#
