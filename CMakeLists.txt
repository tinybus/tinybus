cmake_minimum_required(VERSION 3.20.0)

# check for posix build
set(POSIX_SYSTEM_NAMES "AIX;HP-UX;Darwin;Linux;SCO_SV;UnixWare;OS/360")
if(CMAKE_SYSTEM_NAME IN_LIST POSIX_SYSTEM_NAMES)
  set(POSIX_PLATFORM TRUE)
endif()

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)

set(TINYBUS_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MODULE_PATH "${TINYBUS_DIR}/cmake/platform")

# build the platform specific compoentents
if(ESP_PLATFORM)
  include(esp)
elseif(ZEPHYR_BASE)
  include(zephyr)
elseif(POSIX_PLATFORM)
  include(posix)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()
