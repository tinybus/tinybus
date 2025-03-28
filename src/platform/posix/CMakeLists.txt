cmake_minimum_required(VERSION 3.20)
ty_library_sources(${CMAKE_CURRENT_SOURCE_DIR}/scheduler.c
                     ${CMAKE_CURRENT_SOURCE_DIR}/threadqueue.c)

ty_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CONFIG_TINYBUS_LOG_LEVEL)
  set(CONFIG_TINYBUS_LOG_LEVEL ${CONFIG_TINYBUS_LOG_LEVEL})
else()
  set(CONFIG_TINYBUS_LOG_LEVEL TY_LOG_LEVEL_CRIT)
endif()

ty_library_compile_definitions(
  -DTINYBUS_CONFIG_LOG_LEVEL=${CONFIG_TINYBUS_LOG_LEVEL}
  -DTINYBUS_PLATFORM_CONFIG_FILE="tinybus-posix-config.h")
