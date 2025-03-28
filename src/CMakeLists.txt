cmake_minimum_required(VERSION 3.20)

ty_library_include_directories_public(${CMAKE_CURRENT_SOURCE_DIR})
ty_library_sources(${CMAKE_CURRENT_SOURCE_DIR}/tinybus.c)

add_subdirectory(platform)
