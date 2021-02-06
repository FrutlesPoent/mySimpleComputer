cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-O0 -std=c99")

ADD_LIBRARY(simpleComputer STATIC memory.c register.c memory.h register.h stat.h)

SET(APP_EXE Program)

ADD_EXECUTABLE(${APP_EXE} main.c)

TARGET_LINK_LIBRARIES(${APP_EXE} simpleComputer)