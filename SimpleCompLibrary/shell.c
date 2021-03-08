#include "shell.h"

void set_color(ForegroundColors colorfg, BackgroundColors colorbg){
    mt_setfgcolor(colorfg);
    mt_setbgcolor(colorbg);
}

void sc_wrong_str_memory(char buff[6]){
    buff[0] = '-';
    for (int i = 1; i < 5; i++)
        buff[i] = '0';
    buff[5] = '\0';
}

void get_memory_buff(char buff[6], int command, int operand){
    buff[0] = '+';
    buff[1] = command / 10 + 0x30;
    buff[2] = command % 10 + 0x30;
    buff[3] = operand / 10 + 0x30;
    buff[4] = operand % 10 + 0x30;
    buff[5] = '\0';
}

void memory_index_paint(int index){
    int memValue;
    int command;
    int operand;
    int retval;
    char buff[6];

    retval = sc_memoryGet(index, &memValue);
    if (retval != 0) {
        sc_wrong_str_memory(buff);
        printf("%s",buff);
    }

    retval = sc_commandDecode(memValue, &command, &operand);
    if (retval != 0){
        sc_wrong_str_memory(buff);
        printf("%s",buff);
    }

    get_memory_buff(buff, command, operand);
    printf("%s", buff);
}

void fillTerm(){
    int height;
    int width;

    mt_getscreensize(&height, &width);

    mt_gotoXY(0,0);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++)
            putchar(' ');
        putchar('\n');
    }
}

void shellRun(){ // main
    mt_clrscr();

    set_color(BlackFore,WhiteBack);

    fillTerm();
    memory_paint();
    set_color(BlackFore,WhiteBack);
    flags_paint();
    set_color(BlackFore,WhiteBack);
    keys_paint();
    set_color(BlackFore,WhiteBack);
    operation_paint();
    set_color(BlackFore,WhiteBack);
    instruction_counter_paint();
    set_color(BlackFore,WhiteBack);
    accumulator_paint();
    box_paint();
    mt_gotoXY(0, 100);
}

void memory_paint(){
    bc_box(1,1,62,12);
    mt_gotoXY(26,1);
    printf("Memory");

    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            mt_gotoXY((j * 6) + 2, i + 2);
            memory_index_paint(i * 10 + j);
            putchar(' ');
        }
    }
    mt_gotoXY((positionColShell * 6) + 2, positionRowShell + 2);
    set_color(WhiteFore, BlackBack);
    memory_index_paint(positionRowShell * 10 + positionColShell );
    set_color(WhiteFore, BlackBack);
    putchar(' '); 
}

void flags_paint(){
    int offsetCol = 63;
    int offsetRow = 10;
    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(offsetCol+6, offsetRow);
    printf(" Flags ");
    mt_gotoXY(offsetCol + 7, 1 + offsetRow);
    sc_printReg();
}

void keys_paint(){
    int offsetCol = 48;
    int offsetRow = 13;
    bc_box(offsetCol, offsetRow, 36, 10);
    mt_gotoXY(offsetCol+1, offsetRow);
    printf(" Keys:");
    mt_gotoXY(offsetCol+1, offsetRow+1);
    printf("l - load");
    mt_gotoXY(offsetCol+1, offsetRow+2);
    printf("s - save");
    mt_gotoXY(offsetCol+1, offsetRow+3);
    printf("r - run");
    mt_gotoXY(offsetCol+1, offsetRow+4);
    printf("t - step");
    mt_gotoXY(offsetCol+1, offsetRow+5);
    printf("i - reset");
    mt_gotoXY(offsetCol+1, offsetRow+6);
    printf("F5 - accumulator");
    mt_gotoXY(offsetCol+1, offsetRow+7);
    printf("F6 - instructionCounter");
}

void operation_paint()
{
    int offsetCol = 63;
    int offsetRow = 7;

    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(5 + offsetCol, offsetRow);
    printf(" Operation ");
    mt_gotoXY(7 + offsetCol ,offsetRow + 1);
    printf("+00 : 00");
}

void instruction_counter_paint(){
    int offsetCol = 63;
    int offsetRow = 4;
    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(1 + offsetCol, offsetRow);
    printf("instructionCounter");
    mt_gotoXY(7 + offsetCol ,offsetRow + 1);
    set_color(BlackFore,WhiteBack);
    printf("+0000");
}

void accumulator_paint(){
    int offsetCol = 63;
    int offsetRow = 1;
    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(5 + offsetCol, offsetRow);
    printf("accumulator");
    mt_gotoXY(7 + offsetCol, offsetRow + 1);
    printf("+9999");
}

void get_zero(int value[2]){
    value[0] = 1717976064;
    value[1] = 3958374;
}

void get_one(int value[2]){
    value[0] = 1010315264;
    value[1] = 3158064;
}

void get_two(int value[2]){
    value[0] = 1010842624;
    value[1] = 8258050;
}

void get_three(int value[2]){
    value[0] = 2120252928;
    value[1] = 8282238;
} 

void get_four(int value[2]){
    value[0] = 2120640000;
    value[1] = 6316158;
}

void get_five(int value[2]){
    value[0] = 1040350720;
    value[1] = 4079680;
}

void get_six(int value[2]){
    value[0] = 35789824;
    value[1] = 1974814;
}

void get_seven(int value[2]){
    value[0] = 811630080;
    value[1] = 396312;
}

void get_eight(int value[2]){
    value[0] = 1013332992;
    value[1] = 3958374;
}

void get_nine(int value[2]){
    value[0] = 2087074816;
    value[1] = 3956832;
}

void get_plus(int value[2]){
    value[0] = 2115508224;
    value[1] = 1579134;
}


void box_paint(){
    int offsetRow = 13;
    int offsetCol = 2;
    int valueChar[2];

    bc_box(offsetCol, offsetRow, 46 , 10);
    get_plus(valueChar);
    bc_printBigChar(valueChar, offsetCol + 1 , offsetRow + 1, BlackFore, WhiteBack);
    offsetCol += 9;
    get_nine(valueChar);
    bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
    offsetCol += 9;
    bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
    offsetCol += 9;
    bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
    offsetCol += 9;
    bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
}



