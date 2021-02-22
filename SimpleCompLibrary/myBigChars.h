#include <stdio.h>
#include "terminal.h"
#include <fcntl.h>
#include <errno.h>

#ifndef MYBIGCHARS_H
#define MYBIGCHARS_H

#define BC_ALTCHARSET_A "a" 
#define BC_ALTCHARSET_BOTTOM_RIGHT "j"
#define BC_ALTCHARSET_BOTTOM_LEFT "m" 
#define BC_ALTCHARSET_TOP_RIGHT "k" 
#define BC_ALTCHARSET_TOP_LEFT "l"
#define BC_ALTCHARSET_VERTICAL "x"
#define BC_ALTCHARSET_HORIZONTAL "q"

int bc_printF(char* str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printBigChar(int [2], int x, int y, ForegroundColors, BackgroundColors);
int bc_setBigCharPos(int* big, int x, int y, int value);
int bc_getBigCharPos(int* big, int x, int y, int* value);
int bc_bigCharWrite(int fd, int* big, int count);
int bc_bigCharRead(int fd, int* big, int need_count, int* count);

#endif