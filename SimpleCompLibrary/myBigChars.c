#include <stdio.h>
#include "myBigChars.h"

int bc_printA(char* str){
    
    printf("\E(0%s\E(B", str);
    return 0;
}

int bc_box(int x1, int y1, int x2, int y2){
    	int x, y, i;
	mt_clrscr();
	mt_getscreensize(&y, &x);

	if ((x1 < 0) || (y1 < 0) || (x1+x2 > x) || (y1+y2 > y) || (x2 < 2) || (y2 < 2))
		return -1;

	mt_gotoXY(x1, y1);
	bc_printA(BC_ALTCHARSET_TOP_LEFT);

	for (i = 0; i < x2 - 2; i++)
		bc_printA(BC_ALTCHARSET_HORIZONTAL);

	bc_printA(BC_ALTCHARSET_TOP_RIGHT);

	for (i = 1 ; i < y2-1; i++)
	{
		mt_gotoXY(x1, y1+i);
		bc_printA(BC_ALTCHARSET_VERTICAL);
		mt_gotoXY(x1+x2-1, y1+i);
		bc_printA(BC_ALTCHARSET_VERTICAL);
	}
	mt_gotoXY(x1, y1+y2-1);
	bc_printA(BC_ALTCHARSET_BOTTOM_LEFT);
	for (i = 0; i < x2 - 2; i++)

		bc_printA(BC_ALTCHARSET_HORIZONTAL);

	bc_printA(BC_ALTCHARSET_BOTTOM_RIGHT);
	printf("\n");
	
	return 0;
}

int bc_printBigChar(int* a, int x, int y, ForegroundColors foreColor, BackgroundColors backColor){

    int x1, y1;
    mt_clrscr();
    mt_getscreensize(&y1, &x1);

    if (x < 0 || y < 0)
        return -1;

    mt_setfgcolor(foreColor);
    mt_setbgcolor(backColor);
    
    mt_gotoXY(x,y);    
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if ((a[i/4] >> ((i % 4) * 8 + j -1) & 1))
                bc_printA(BC_ALTCHARSET_A);
            else
                printf(" ");
            mt_gotoXY(x + j, y + i);
        }

    }

 mt_setfgcolor(WhiteFore);
 mt_setbgcolor(BlackBack);
}

int bc_setBigCharPos(int* big, int x, int y, int value){

    if ((x < 0) || (y < 0) || (x > 7) || (y > 7) || (value < 0) || (value > 1))
		return -1;
	if (value == 0)
		big[y/4] &= ~(1 << (8 * (y % 4) + x));
	else
		big[y/4] |= 1 << (8 * (y % 4) + x);

	return 0;
}
int bc_getBigCharPos(int* big, int x, int y, int* value){

    if ((x < 0) || (y < 0) || (x > 7) || (y > 7))
		return -1;

	*value = big[y / 4] >> 8 * (y % 4) + x;

	return 0;
}

int bc_bigCharWrite(int fd, int* big, int count){
    if (!big || fd < 0){
        return -1;
    }

    while (count > 0){
        int result;
        if ((result = write(fd,big,sizeof(int) * 2)) == -1)
            return -1;
        count -= 1;
    }
    
    return 0;
}

int bc_bigCharRead(int fd, int* big, int need_count, int* count){
    if (fd < 0)
        return -1;
    while(need_count > 0){
        int result;
        if ((result = read(fd, big, sizeof(int) * 2)) == -1)
            return -1;
        need_count -= 1;
        *count = *count + 1;
    }

    return 0;
}
