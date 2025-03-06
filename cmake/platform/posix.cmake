cmake_minimum_required(VERSION 3.20)

add_library(tinybus INTERFACE)
add_library(tinybus-platform INTERFACE)
target_link_libraries(tinybus INTERFACE tinybus-platform)
target_include_directories(tinybus INTERFACE ${TINYBUS_DIR}/include)
add_subdirectory(${TINYBUS_DIR}/src)
