#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>

#ifndef TERMINAL_H
#define TERMINAL_H

typedef enum {
    BlackFore = 0,
    RedFore = 1,
    GreenFore = 2,
    YellowFore = 3,
    BlueFore = 4,
    MagentaFore = 5,
    CyanFore = 6,
    WhiteFore = 7
} ForegroundColors;

typedef enum {
    BlackBack = 0,
    RedBack = 1,
    GreenBack = 2,
    YellowBack = 3,
    BlueBack = 4,
    MagentaBack = 5,
    CyanBack = 6,
    WhiteBack = 7
} BackgroundColors;

int mt_clrscr();
int mt_gotoXY(int x, int y);
int mt_getscreensize(int* rows, int* cols);
int mt_setfgcolor(ForegroundColors);
int mt_setbgcolor(BackgroundColors);
int mt_default();

#endif