
project (Gamestream)
cmake_minimum_required (VERSION 3.6)

#Debug mode
set(CMAKE_BUILD_TYPE Debug)
#Find all files in source(gamestream / src) directory
file (GLOB SRC_DIR "*.hpp" "*.cpp")
add_executable (Gamestream ${SRC_DIR})
#Include directories and link libs(source, xcb)
include_directories (${PROJECT_SOURCE_DIR} /usr/include /usr/include/xcb/ /usr/include/libavcodec /usr/include/libavformat /usr/include/c++/6.3.1 /usr/include/libswscale)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -iquote /usr/include/libavutil/")
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ofast")
endif (CMAKE_BUILD_TYPE STREQUAL "Release")

link_directories (/usr/lib64 /usr/lib)
target_link_libraries (Gamestream xcb avutil avcodec avformat swscale)

#Debug section
message (STATUS ${PROJECT_SOURCE_DIR})
