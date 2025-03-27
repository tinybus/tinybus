cmake_minimum_required(VERSION 3.20)
tiny_library_sources(${CMAKE_CURRENT_SOURCE_DIR}/scheduler.c
                     ${CMAKE_CURRENT_SOURCE_DIR}/threadqueue.c)

tiny_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

tiny_compile_definitions(
  -DTINYBUS_CONFIG_LOG_LEVEL=${CONFIG_TINYBUS_LOG_LEVEL}
  -DTINYBUS_PLATFORM_CONFIG_FILE="tinybus-posix-config.h")
