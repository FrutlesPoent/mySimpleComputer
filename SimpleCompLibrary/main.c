#include <stdio.h>
#include "memory.h"
#include "register.h"
#include "terminal.h"
#include "myBigChars.h"
#include "shell.h"

int main() {

    // int test;

    // sc_memoryInit();
    // sc_regInit();

    // int commandMy;
    // int result = sc_commandEncode(10,50, &commandMy);
    // printf("True result encode = %d\n", result); // 0= true, 1 - false

    // result = sc_commandEncode(12,128, &commandMy);
    // printf("False result encode = %d\n", result);

    // result = sc_memorySet(50, commandMy);
    // printf("True Memory result = %d\n",result);

    // result = sc_memorySet(101, commandMy);
    // printf("False memory result = %d", result);

    // sc_memorySave("file.bin");
    // sc_memoryInit();
    // sc_memoryLoad("file.bin");

    // result = sc_memoryGet(0, &commandMy);
    // printf("%d", result);

    // mt_clrscr();
    // mt_setbgcolor(CyanBack);
    // mt_setfgcolor(RedFore);
    // printf("HelloRED");
    // mt_default();
    // mt_gotoXY(5,3);
    // printf("lul");

    // int rows;
    // int cols;

    // mt_getscreensize(&rows, &cols);
    // printf("rows: %d and cols: %d", rows, cols);
    // bc_box(6,6,5,5);

    //int fd = open("file.bin", O_CREAT | O_RDWR | O_TRUNC);

    //int a[2] = {606348348, 1009001508};
    //bc_setBigCharPos(a, 0, 0, 1);
    //bc_bigCharWrite(fd, a, 2);
    //int fo = open("file.bin", O_RDONLY);
    //bc_printBigChar(a, 2, 3, WhiteFore, BlackBack);
    // int test;
    // bc_getBigCharPos(a, 1, 0, &test);
    // printf("\n%d", test);

    sc_memoryInit();
    sc_regInit();
    shellRun();
    return 0;

}
