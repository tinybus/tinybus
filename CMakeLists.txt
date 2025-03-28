cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)

set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(TYCOMMON_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/tycommon)
set(CMAKE_MODULE_PATH "${PROJECT_DIR}/platform" "${TYCOMMON_DIR}/cmake")

# add build extensions for tinylibs
include(extensions)

# build the platform specific compoentents
if(ESP_PLATFORM)
  include(esp)
elseif(ZEPHYR_BASE)
  include(zephyr)
elseif(UNIX)
  include(posix)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()
