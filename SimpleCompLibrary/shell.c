#include "shell.h"

void paintCell();
// typedef void (*sighandler_t)(int);

// sighandler_t signal(int signum, sighandler_t handler){
//     printf("True");
// }

static void timerTimeoutHandler(int a){
    (void)a;
    int clockFlag;
    sc_regGet(IMP, &clockFlag);
    if (clockFlag)
        return;
    sc_counter_set((sc_counter_get() + 1) % SIZE);
    instruction_counter_paint(0);
}

void set_color(ForegroundColors colorfg, BackgroundColors colorbg){
    mt_setfgcolor(colorfg);
    mt_setbgcolor(colorbg);
}

void inputMemory(){
    printf("Input value");
    rk_mytermregime(1, 0, 0, 1, 1);
    int command, operand, result;
    scanf("%2d%2d", &command, &operand);
    int retval = sc_commandEncode(command, operand, &result);
    if (retval != 0)
        printf("Input error\n");
    else
        sc_memorySet((positionRowShell * 10 + positionColShell), result);

    rk_mytermregime(0, 0, 0, 0, 1);
}

void instruction_iter(){
 int value = sc_counter_get();
 int valueReg;
 sc_regGet(IMP, &valueReg);

 if (!valueReg){
      value += 1;
 }

 sc_counter_set(value);
}


void input_instruction(){
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 0);
    //int command, operand, result;
    int value;
    scanf("%4d", &value);
    if (value > 9999){
        printf("Input error");
        rk_mytermregime(0,0,0,0,1);
    }
    sc_counter_set(value);
    rk_mytermregime(0, 0, 0, 0, 1);
}

void input_accumulator(){
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 0);
    int command, operand, result;
    scanf("%2d%2d", &command, &operand);
    int retval = sc_commandEncode(command, operand, &result);
    if (retval != 0) {
        printf("Input error");
        rk_mytermregime(0, 0, 0, 0, 1);;
    }
    sc_accum_set(result);
    rk_mytermregime(0, 0, 0, 0, 1);
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
        printf("%s", buff);
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

void paintCell(){
    fillTerm();
    memory_paint();
    set_color(BlackFore,WhiteBack);
    flags_paint();
    set_color(BlackFore,WhiteBack);
    keys_paint();
    set_color(BlackFore,WhiteBack);
    operation_paint();
    set_color(BlackFore,WhiteBack);
    instruction_counter_paint(1);
    set_color(BlackFore,WhiteBack);
    accumulator_paint();
    set_color(BlackFore,WhiteBack);
    box_paint();
}

void signalhandler(int signo){
    if (signo == SIGALRM){
        instruction_iter();
        instruction_counter_paint(0);
        paintCell();
    }
    if (signo == SIGUSR1){
        printf("restart\n");
        sc_memoryLoad("reset.bin");
        sc_regInit();
    }
}

void check_signal() {
    if (signal(SIGALRM, signalhandler) == SIG_ERR)
        printf("\n cant't catch SIGINT\n");
    if (signal(SIGUSR1, signalhandler) == SIG_ERR)
        printf("\n cant't catch SIGUSR1\n");
}

void reRun(){
    sc_memoryLoad("reset.bin");
    sc_regInit();
    sc_memoryInit();
    sc_counter_set(0);
    paintCell();
    shellRun();

}

int shellRun(){ // main

    struct sigaction act;
    act.sa_handler = &timerTimeoutHandler;
    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);

    struct itimerval nval, oval;

    nval.it_interval.tv_sec = 30;
    nval.it_interval.tv_usec = 500;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &nval, &oval);
    //check_signal();
    mt_clrscr();

    set_color(BlackFore,WhiteBack);
    rk_mytermregime(0,0,0,0,1);
    paintCell();

    mt_gotoXY(1,25);
    enum Keys keys;
    while(1){
        check_signal();
        rk_readkey(&keys);
        switch (keys){
        case KEY_right: {
            if (positionColShell < 9){
                positionColShell += 1;
                paintCell();
            }
            break;
        }
        case KEY_left :{
            if (positionColShell > 0){
                positionColShell -= 1;
                paintCell();
            }
            break;
        }
        case KEY_up: {
            if (positionRowShell > 0) {
                positionRowShell -= 1;
                paintCell();
            }
            break;
        }
        case KEY_down: {
            if (positionRowShell < 9){
                positionRowShell += 1;
                paintCell();
            }
            break;
        }
        case KEY_wrong: {
            break;
        }
        case KEY_f5: {
            mt_gotoXY(1, 25);
            input_accumulator();
            getchar();
            accumulator_paint();
            paintCell();
            break;
        }
        case KEY_f6: {
            mt_gotoXY(1,25);
            input_instruction();
            getchar();
            //instruction_counter_paint();
            paintCell();
            break;
        }
        case KEY_l: {
            mt_gotoXY(1, 25);
            rk_mytermregime(1, 0, 0, 1, 1);
            printf("file name: \n");
            char filename[100];
            scanf("%s", filename);
            getchar();
            rk_mytermregime(0, 0, 0, 0, 1);
            sc_memoryLoad(filename);
            paintCell();
            break;        
            }
        case KEY_i: {
            sc_memoryLoad("reset.bin");
            sc_regInit();
            paintCell();
            break;
        }
        case KEY_s: {
            mt_gotoXY(1, 25);
            rk_mytermregime(1, 0, 0, 1, 1);
            printf("filename: \n");
            char filename[100];
            scanf("%s", filename);
            getchar();
            rk_mytermregime(0, 0, 0, 0, 1);
            sc_memorySave(filename);
            paintCell();
            break;

        }

        case KEY_r : {
            raise(SIGUSR1);
            reRun();
            break;
        }
        case KEY_enter: {
            mt_gotoXY(1, 25);
            inputMemory();
            getchar();
            paintCell();
            break;
        }
        }
    }

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
    set_color(BlackFore, WhiteBack);
}

void flags_paint(){
    int offsetCol = 63;
    int offsetRow = 10;
    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(offsetCol + 6, offsetRow);
    printf(" Flags ");
    mt_gotoXY(offsetCol + 7, 1 + offsetRow);
    flagsRegPaint();
}

void flagsRegPaint(){
    int value;
    char buff[5];
    for (int i = 0 ; i < 5; i++)
        buff[i] = ' ';
     
    sc_regGet(OVERFLOW,&value);
    if (value)
        buff[0] = 'P';
    sc_regGet(DELZERO, &value);
    if (value)
        buff[1] = 'O';
    sc_regGet(OUTMEM, &value);
    if (value)
        buff[2] = 'M';
    sc_regGet(IMP, &value);
    if (value)
        buff[3] = 'T';
    sc_regGet(COMMAND, &value);
    if (value)
        buff[4] = 'E';
    for (int i = 0; i < 5; i++){
        putchar(buff[i]);
        putchar(' ');
    }
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

void get_mem_buff(char buff[6], int value){
    buff[0] = '+';
    buff[4] = value % 10 + 0x30;
    value = value / 10;
    buff[3] = value % 10 + 0x30 ;
    value = value / 10;
    buff[2] = value % 10 + 0x30;
    value = value / 10;
    buff[1] = value % 10 + 0x30;
    buff[5] = '\0';
}

void instruction_counter_paint(int a){
    int offsetCol = 63;
    int offsetRow = 4;
    //int command;
    //int operand;
    char buff[6];
    int value = sc_counter_get();

    //sc_commandDecode(value, &command, &operand);
    if (a){
        bc_box(offsetCol, offsetRow, 20, 3);
        mt_gotoXY(1 + offsetCol, offsetRow);
        printf("instructionCounter");
        mt_gotoXY(7 + offsetCol ,offsetRow + 1);
        set_color(BlackFore,WhiteBack);
    }

    //get_memory_buff(buff, command, operand);
    get_mem_buff(buff, value);
    
    printf("%s", buff);
}

void accumulator_paint(){
    int offsetCol = 63;
    int offsetRow = 1;
    int value = sc_accum_get();
    int command;
    int operand;
    char buff[6];

    sc_commandDecode(value, &command, &operand);

    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(5 + offsetCol, offsetRow);
    printf("accumulator");
    mt_gotoXY(7 + offsetCol, offsetRow + 1);

    get_memory_buff(buff, command, operand);

    printf("%s", buff);
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

void get_minus(int value[2]){
    value[0] = 2113929216;
    value[1] = 126;
}


void switchValue(int value, int valueChar[2]){
    switch (value) {
    case 0:
        get_zero(valueChar);
        return;
    case 1:
        get_one(valueChar);
        return;
    case 2:
        get_two(valueChar);
        return;
    case 3:
        get_three(valueChar);
        return;
    case 4:
        get_four(valueChar);
        return;
    case 5:
        get_five(valueChar);
        return;
    case 6:
        get_six(valueChar);
        return;
    case 7:
        get_seven(valueChar);
        return;
    case 8:
        get_eight(valueChar);
        return;
    case 9:
        get_nine(valueChar);
        return;
    }
}

void box_paint(){
    int offsetRow = 13;
    int offsetCol = 2;
    int valueChar[2];
    int memoryValue;
    int command;
    int operand;

    bc_box(offsetCol, offsetRow, 46 , 10);
    sc_memoryGet(positionRowShell * 10 + positionColShell, &memoryValue);
    if (sc_commandDecode(memoryValue, &command, &operand) != 0){
        command = 0;
        operand = 0;
        get_minus(valueChar);
        bc_printBigChar(valueChar, offsetCol + 1 , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
    } else {
        get_plus(valueChar);
        bc_printBigChar(valueChar, offsetCol + 1 , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(command / 10, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(command % 10, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(command / 10, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(command % 10, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
    }
    mt_gotoXY(1, 25);
}


