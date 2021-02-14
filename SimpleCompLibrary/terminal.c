#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "terminal.h"

int mt_clrscr(){

    printf("\E[H\E[J");

    return 0;
}

int mt_default(){
    
    printf("\E[0m");

    return 0; 
}

int mt_gotoXY(int x, int y){

    printf("\E[%d;%dH", y, x); // first row, second  column

    return 0;
}

int mt_getscreensize(int* rows, int* cols){

    struct winsize size;

    if (!ioctl(1, TIOCGWINSZ, &size)){
        *rows = size.ws_row;
        *cols = size.ws_col;
    } else {
        return -1;
    }

    return 0;
}

int mt_setfgcolor(ForegroundColors color){

    printf("\E[3%dm", color);

    return 0;
}

int mt_setbgcolor(BackgroundColors color) {

    printf("\E[4%dm", color);

    return 0;
}