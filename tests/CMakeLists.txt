get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} Tests.cpp)
target_link_libraries(${PROJECT_NAME} rtc::core doctest::doctest rtc tutorials)
