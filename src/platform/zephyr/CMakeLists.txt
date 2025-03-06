cmake_minimum_required(VERSION 3.20)

target_sources(
  tinybus-platform
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/platform.c
            ${CMAKE_CURRENT_SOURCE_DIR}/scheduler.c
            ${CMAKE_CURRENT_SOURCE_DIR}/logging.c)

target_include_directories(tinybus-platform
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(
  tinybus-platform
  INTERFACE -DTINYBUS_CONFIG_LOG_LEVEL=${CONFIG_TINYBUS_LOG_LEVEL}
            -DTINYBUS_PLATFORM_CONFIG_FILE="tinybus-zephyr-config.h")
