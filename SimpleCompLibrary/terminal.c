#include <stdio.h>
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

    printf("\E[%d;%dH", x, y);

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