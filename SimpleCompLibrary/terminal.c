#include <stdio.h>
#include "terminal.h"

int mt_clrscr(){

    printf("\E[H\E[J");
    
    return 0;
}