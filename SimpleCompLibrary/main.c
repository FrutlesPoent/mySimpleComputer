#include <stdio.h>
#include "memory.h"
#include "register.h"
#include "terminal.h"

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

    mt_clrscr();
    mt_setbgcolor(CyanBack);
    mt_setfgcolor(RedFore);
    printf("HelloRED");
    mt_default();
    mt_gotoXY(5,3);
    printf("lul");


    return 0;

}
