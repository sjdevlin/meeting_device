project(meeting_device)
cmake_minimum_required(VERSION 2.4.6)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_FFTW3 REQUIRED fftw3f)
pkg_check_modules(PC_ALSA REQUIRED alsa)
pkg_check_modules(PC_LIBCONFIG REQUIRED libconfig)
pkg_check_modules(PC_GLIB REQUIRED glib-2.0)
pkg_check_modules(PC_GIO REQUIRED gio-2.0)
pkg_check_modules(PC_GOB REQUIRED gobject-2.0)

set(CMAKE_BUILD_TYPE "Debug") 
#remember to change to release !
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

#Add base directory for includes (global)

add_executable(meetpie
    demo/odaslive/main.cpp
    demo/odaslive/audio.cpp
    demo/odaslive/ble.cpp
    demo/odaslive/configs.c
    demo/odaslive/objects.c
    demo/odaslive/parameters.c
    demo/odaslive/threads.c
)

target_link_libraries(meetpie
    odas
	${PC_FFTW3_LIBRARIES}
	${PC_ALSA_LIBRARIES}
    ${PC_LIBCONFIG_LIBRARIES}
	m
    pthread
    glib-2.0
	gio-2.0
	gobject-2.0
    libc.so.6
    libm.so.6
	/home/pi/odas/lib/libggk.a

)


