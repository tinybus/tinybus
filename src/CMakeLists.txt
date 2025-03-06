cmake_minimum_required(VERSION 3.20)

target_sources(tinybus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tinybus.c)
target_include_directories(tinybus INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(platform)
