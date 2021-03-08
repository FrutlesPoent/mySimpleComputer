cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-O0 -std=c99")

ADD_LIBRARY(simpleComputer STATIC memory.c register.c memory.h register.h stat.h)
ADD_LIBRARY(myTerm STATIC terminal.c terminal.h)
ADD_LIBRARY(BigChars STATIC myBigChars.c myBigChars.h)
ADD_LIBRARY(ShellMy STATIC shell.c shell.h)

SET(APP_EXE Program)

ADD_EXECUTABLE(${APP_EXE} main.c)
TARGET_LINK_LIBRARIES(BigChars myTerm)
TARGET_LINK_LIBRARIES(${APP_EXE} ShellMy BigChars)

TARGET_LINK_LIBRARIES(${APP_EXE} simpleComputer BigChars)