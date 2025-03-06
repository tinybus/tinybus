cmake_minimum_required(VERSION 3.20)

# CHECK needed for esp-idf build system to avoid errors
if(NOT CMAKE_BUILD_EARLY_EXPANSION)
  add_library(tinybus INTERFACE)
  add_library(tinybus-platform INTERFACE)
  target_link_libraries(tinybus INTERFACE tinybus-platform)
  target_include_directories(tinybus INTERFACE ${TINYBUS_DIR}/include)
  add_subdirectory(${TINYBUS_DIR}/src)
endif()

idf_component_register()
target_link_libraries(${COMPONENT_LIB} INTERFACE tinybus)
