
project (Gamestream)
cmake_minimum_required (VERSION 3.7)

#Find all files in source(gamestream / src) directory
file (GLOB SRC_DIR "*.hpp" "*.cpp")
add_executable (Gamestream ${SRC_DIR})

#Include directories and link libs(source, xcb)
include_directories (${PROJECT_SOURCE_DIR} /usr/include/xcb/)
link_directories (/usr/lib64/ /usr/lib)
target_link_libraries (Gamestream xcb)

#Debug section
message (STATUS ${PROJECT_SOURCE_DIR})
