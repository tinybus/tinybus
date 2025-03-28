cmake_minimum_required(VERSION 3.20)
ty_library_sources(${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/threadqueue.c)

ty_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CONFIG_TYBUS_LOG_LEVEL)
  set(CONFIG_TYBUS_LOG_LEVEL ${CONFIG_TYBUS_LOG_LEVEL})
else()
  set(CONFIG_TYBUS_LOG_LEVEL TY_LOG_LEVEL_CRIT)
endif()

ty_library_compile_definitions(
  -DTYBUS_CONFIG_LOG_LEVEL=${CONFIG_TYBUS_LOG_LEVEL}
  -DTYBUS_PLATFORM_CONFIG_FILE="tybus-posix-config.h")
