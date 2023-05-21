cmake_minimum_required(VERSION 3.25)
project(RGR_cruses_v2)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc)

add_executable(RGR_cruses_v2
        inc/keys.h
        inc/libGameRGR2.h
        inc/local.h
        lib/error.o
        lib/libGameRGR2.o
        lib/libGameRGR2.so
        lib/utils.o
        src/error.c
        src/libGameRGR2.c
        src/utils.c
        test.c)
