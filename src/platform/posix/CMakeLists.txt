cmake_minimum_required(VERSION 3.20)
tiny_library_sources(${CMAKE_CURRENT_SOURCE_DIR}/scheduler.c)

tiny_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

tiny_compile_definitions(
  -DTINYSETTINGS_CONFIG_LOG_LEVEL=${CONFIG_TINYBUS_LOG_LEVEL}
  -DTINYSETTINGS_PLATFORM_CONFIG_FILE="tinybus-posix-config.h")
