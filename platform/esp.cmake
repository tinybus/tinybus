cmake_minimum_required(VERSION 3.20)

idf_component_register()

tiny_library_named(tinybus)
tiny_include_directories(${TINYBUS_DIR}/include)
add_subdirectory(${TINYBUS_DIR}/src)
target_link_libraries(${COMPONENT_LIB} INTERFACE tinybus)
