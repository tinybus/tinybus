cmake_minimum_required(VERSION 3.20)

tiny_library_named(tinybus)
tiny_library_link_libraries(zephyr_interface)

zephyr_library()
zephyr_library_sources(${TINYCOMMON_DIR}/src/empty_file.c)

tiny_include_directories(${TINYBUS_DIR}/include)

add_subdirectory(${TINYBUS_DIR}/src)
zephyr_link_libraries(tinybus)
