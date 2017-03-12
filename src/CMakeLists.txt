#X11 option if linux
option(GS_USE_X11 "Use X11 for linux graphics" ON)

project (Gamestream)
cmake_minimum_required (VERSION 3.6)

#Debug mode
#set(CMAKE_BUILD_TYPE Debug)
set(SRC_DIR gamestream.cpp encoder.cpp capture/gs_capture.cpp decode_test.cpp)
set(TARGET_LIBS stdc++ xcb avutil avcodec avformat swscale)

#If linux
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(TARGET_LIBS ${TARGET_LIBS} pthread)
	#Set GS_USE_X11 if building for X11
	if (GS_USE_X11)
		set(SRC_DIR ${SRC_DIR} capture/x11_capture.cpp)
		set(COMP_DEF GS_USE_X11=1)
		message(STATUS "Using X11 on GNU/Linux")
	else (GS_USE_X11)
		set(SRC_DIR ${SRC_DIR} capture/gs_capture.cpp)
		message(STATUS "Using Generic WM on GNU/Linux")
	endif (GS_USE_X11)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")


#TESTING
set(SRC_DIR ${SRC_DIR} sched/gs_sched.cpp)

#Add executables
add_executable (Gamestream ${SRC_DIR})

#Add compiler definitions
target_compile_definitions(Gamestream PUBLIC ${COMP_DEF})

#TODO Add finders
#Include directories and link libs(source, xcb)
include_directories (${PROJECT_SOURCE_DIR} /usr/include /usr/include/xcb/ /usr/include/libavcodec /usr/include/libavformat /usr/include/libswscale)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -iquote /usr/include/libavutil/ -pthread -std=c++14")

#Increase optimization level for release builds
if (CMAKE_BUILD_TYPE STREQUAL "Release")
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ofast")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ofast")
endif (CMAKE_BUILD_TYPE STREQUAL "Release")

#Debug flags
if (CMAKE_BUILD_TOOL STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0")
endif (CMAKE_BUILD_TOOL STREQUAL "Debug")

link_directories (/usr/lib64 /usr/lib)
target_link_libraries (Gamestream ${TARGET_LIBS})


#NOTES
#Auto include file (GLOB SRC_DIR "*.hpp" "*.cpp")
