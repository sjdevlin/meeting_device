project(meeting_device)
cmake_minimum_required(VERSION 2.4.6)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_FFTW3 REQUIRED fftw3f)
pkg_check_modules(PC_ALSA REQUIRED alsa)
pkg_check_modules(PC_PULSEAUDIO REQUIRED libpulse-simple)
pkg_check_modules(PC_LIBCONFIG REQUIRED libconfig)
pkg_check_modules(PC_GLIB REQUIRED glib-2.0)
pkg_check_modules(PC_GIO REQUIRED gio-2.0)
pkg_check_modules(PC_GOB REQUIRED gobject-2.0)

include_directories (${GLIB_INCLUDE_DIRS})
link_directories (${GLIB_LIBRARY_DIRS})
include_directories("/home/pi/odas/include/odas")

set(CMAKE_BUILD_TYPE "Release") 
#set(CMAKE_BUILD_TYPE "Debug") 
#remember to change to release !
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -I/home/pi/odas/include/odas/")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

#Add base directory for includes (global)

add_executable(meetpie
    src/main.cpp
    src/audio.cpp
    src/ble.cpp
    src/device.cpp
    src/parameters.c
    src/configs.c
    src/objects.c
    src/threads.c
#    src/mini_gpio.c
)

target_link_libraries(meetpie
    /home/pi/odas/lib/libodas.so
	/home/pi/gobbledegook/src/libggk.a
    ${PC_PULSEAUDIO_LIBRARIES}
	${PC_FFTW3_LIBRARIES}
	${PC_ALSA_LIBRARIES}
    ${PC_LIBCONFIG_LIBRARIES}
    ${PC_GLIB_LIBRARIES}
	m
    pthread
	gio-2.0
	gobject-2.0
    libc.so.6
    libm.so.6
    pigpio
    rt
)


