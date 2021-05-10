#include "shell.h"

int operCommand;

void paintCell();

void set_color(ForegroundColors colorfg, BackgroundColors colorbg){
    mt_setfgcolor(colorfg);
    mt_setbgcolor(colorbg);
}

void inputMemory(){
    printf("Input number or command\n");
    rk_mytermregime(1, 0, 0, 1, 1);
    int choose;
    scanf("%d", &typeOfCommand[positionRowShell * 10 + positionColShell]);
    int command, operand, result;
    if (!typeOfCommand[positionRowShell * 10 + positionColShell]){
        scanf("%2d%2d", &command, &operand);
        int retval = sc_commandEncode(command, operand, &result);
        if (retval != 0)
            printf("Input error\n");
        else
            sc_memorySet((positionRowShell * 10 + positionColShell), result);
    } else {
        scanf("%4d", &result);
        sc_memorySet((positionRowShell * 10 + positionColShell), result + 32768 );
    }

    rk_mytermregime(0, 0, 0, 0, 1);
}
void instruction_iter(){
    int value = sc_counter_get();
    int valueReg;
    int valueReg2;
    sc_regGet(IMP, &valueReg2);
    sc_regGet(IMP, &valueReg);

    if (!(valueReg && valueReg2)){
        value += 1;
    }
    if (value == 100)
        value = 0;
    sc_counter_set(value);

    positionColShell = value % 10;
    positionRowShell = value / 10;

}


void input_instruction(){
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 0);
    //int command, operand, result;
    int value;
    scanf("%2d", &value);
    if (value > 99){
        printf("Input error");
        rk_mytermregime(0,0,0,0,1);
    }
    sc_counter_set(value);
    positionColShell = value % 10;
    positionRowShell = value / 10;
    rk_mytermregime(0, 0, 0, 0, 1);
}

void input_accumulator(){
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 0);
    int command, operand, result;
    int number;
    scanf("%4d", &number);
    int retval = sc_commandEncode(command, operand, &result);
    if (retval != 0) {
        printf("Input error");
        rk_mytermregime(0, 0, 0, 0, 1);;
    }
    sc_accum_set(number);
    rk_mytermregime(0, 0, 0, 0, 1);
}

void sc_wrong_str_memory(char buff[7]){
    buff[0] = '-';
    for (int i = 1; i < 6; i++)
        buff[i] = '0';
    buff[6] = '\0';
}

void get_memory_buff(char buff[6], int command, int operand){
    sprintf(buff, "+%02X%02X", command, operand); // same as itoa, but work
}

void get_memory_buff_number(char buff[6], int value){
    sprintf(buff, "-%04X", value);
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
    if (memValue < 32768 ){
        retval = sc_commandDecode(memValue, &command, &operand);
        if (retval != 0){
            sc_wrong_str_memory(buff);
            printf("%s",buff);
        }
        get_memory_buff(buff, command, operand);
    } else {
        memValue -= 32768; 
        get_memory_buff_number(buff, memValue);
    }

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
    instruction_counter_paint(1);
    set_color(BlackFore,WhiteBack);
    accumulator_paint();
    set_color(BlackFore,WhiteBack);
    operation_paint(positionRowShell * 10 + positionColShell);
    set_color(BlackFore,WhiteBack);
    box_paint();
}

void signalhandler(int signo){
    int clockFlag;
    sc_regGet(IMP, &clockFlag);
    if (clockFlag)
        return;
    if (signo == SIGALRM){
        cu();
        instruction_iter();
        // instruction_counter_paint(0);
    
        mt_gotoXY(1, 25);
    }
    if (signo == SIGUSR1){
        printf("restart\n");
        sc_memoryLoad("reset.bin");
        sc_regInit();
    }
}

void stepCU(){
    cu();
    instruction_iter();
    instruction_counter_paint(0);
    paintCell();
}

void check_signal() {
    if (signal(SIGALRM, signalhandler) == SIG_ERR)
        printf("\n cant't catch SIGINT\n");
    if (signal(SIGUSR1, signalhandler) == SIG_ERR)
        printf("\n cant't catch SIGUSR1\n");
}

void reRun(){
    sc_memoryLoad("reset.bin");
    sc_regSet(8, 1);
    sc_regInit();
    sc_memoryInit();
    sc_counter_set(0);
    paintCell();
    shellRun();

}

int shellRun(){ // main
    sc_regInit();
    struct sigaction act;
    act.sa_handler = &signalhandler;
    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sc_regSet(IMP, 1);

    struct itimerval nval;

    nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = 500;
    nval.it_value.tv_sec = 0;
    nval.it_value.tv_usec = 500;
    setitimer(ITIMER_REAL, &nval, NULL);
    

    mt_clrscr();
    set_color(BlackFore,WhiteBack);
    rk_mytermregime(0,0,0,0,1);
    paintCell();
    paintCell();

    mt_gotoXY(1,25);
    enum Keys keys;
    while(1){ 
        rk_readkey(&keys);
        switch (keys){
        case KEY_right: {
            if (positionColShell < 9){
                positionColShell += 1;
                sc_counter_set(sc_counter_get() + 1);
                paintCell();
            }
            break;
        }
        case KEY_left :{
            if (positionColShell > 0){
                positionColShell -= 1;
                sc_counter_set(sc_counter_get() - 1);
                paintCell();
            }
            break;
        }
        case KEY_up: {
            if (positionRowShell > 0) {
                positionRowShell -= 1;
                sc_counter_set(sc_counter_get() - 10);
                paintCell();
            }
            break;
        }
        case KEY_down: {
            if (positionRowShell < 9){
                positionRowShell += 1;
                sc_counter_set(sc_counter_get() + 10);
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
            instruction_counter_paint();
            paintCell();
            break;
        }
        case KEY_l: {
            mt_gotoXY(1, 25);
            rk_mytermregime(1, 0, 0, 1, 1);
            printf("file name:\n");
            char fileName[250];
            scanf("%s", fileName);
            getchar();
            rk_mytermregime(0, 0, 0, 0, 1);
            sc_memoryLoad(fileName);
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
            sc_regSet(IMP, 0);
            paintCell();
            break;
        }
        case KEY_enter: {
            int value;
            sc_regGet(IMP, &value);
            sc_regSet(IMP, 1);
            mt_gotoXY(1, 25);
            inputMemory();
            getchar();
            if (!value)
                sc_regSet(IMP, 0);
            paintCell();
            break;
        }
        case KEY_t:{
            sc_regSet(IMP, 0);
            stepCU();
            sc_regSet(IMP, 1);
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

void get_oper_buff(char buff[9], int value){
    sprintf(buff, "-%04d", value);
}

void get_oper_buff_command(char buff[9], int command, int operand){
    sprintf(buff, "+%02d : %02d", command, operand);
}

void sc_wrong_str_oper(char buff[7]){
    buff[0] = '-';
    for (int i = 1; i < 6; i++)
        buff[i] = '0';
    buff[3] = ':';
    buff[6] = '\0';
}

void operation_paint(int index)
{
    int memValue;
    int command;
    int operand;
    int retval;
    int offsetCol = 63;
    int offsetRow = 7;
    char buff[9];

    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(5 + offsetCol, offsetRow);
    printf(" Operation ");
    mt_gotoXY(7 + offsetCol, offsetRow + 1);
    retval = sc_memoryGet(index, &memValue);
    if (retval != 0) {
        sc_wrong_str_oper(buff);
        printf("%s", buff);
    }
    if (memValue < 32768)
    {
        retval = sc_commandDecode(memValue, &command, &operand);
        if (retval != 0) {
            sc_wrong_str_oper(buff);
            printf("%s", buff);
        }
        get_oper_buff_command(buff, command, operand);
    } else {
        memValue -= 32768;
        get_oper_buff(buff, memValue);
    }
    printf("%s", buff);
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
    printf("%d", value);

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

void get_memory_accum(char buff[6], int value){
    sprintf(buff, "-%04d", value);
}

void get_memory_buff_accum_plus(char buff[6], int value){
    sprintf(buff, "+%04d", value);
}

void accumulator_paint(){
    int offsetCol = 63;
    int offsetRow = 1;
    int value = sc_accum_get();
    int command;
    int operand;
    char buff[6];


    bc_box(offsetCol, offsetRow, 20, 3);
    mt_gotoXY(5 + offsetCol, offsetRow);
    printf("accumulator");
    mt_gotoXY(7 + offsetCol, offsetRow + 1);

    if (value >= 0)
        get_memory_buff_accum_plus(buff, value);
    else 
        get_memory_accum(buff, value);

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

void get_A(int value[2]) {
    value[0] = 2118269952 ;
    value[1] = 4342338;
}

void get_B(int value[2]) {
    value[0] = 1044528640 ;
    value[1] = 4080194;
}

void get_C(int value[2]) {
    value[0] = 37895168 ;
    value[1] = 3949058;
}

void get_D(int value[2]) {
    value[0] = 1111637504 ;
    value[1] = 4080194;
}

void get_E(int value[2]) {
    value[0] = 2114092544 ;
    value[1] = 8258050;
}

void get_F(int value[2]) {
    value[0] = 33717760 ;
    value[1] = 131646;
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
    case 10:
        get_A(valueChar);
        return;
    case 11:
        get_B(valueChar);
        return;
    case 12: 
        get_C(valueChar);
        return;
    case 13:
        get_D(valueChar);
        return;
    case 14:
        get_E(valueChar);
        return;
    case 15:
        get_F(valueChar);
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

    if (memoryValue >= 32768){
        memoryValue -= 32768;
        int val1,val2, val3, val4;
        val4 = memoryValue % 16;
        memoryValue = memoryValue / 16;
        val3 = memoryValue % 16;
        memoryValue = memoryValue / 16;
        val2 = memoryValue % 16;
        memoryValue = memoryValue / 16;
        val1 = memoryValue % 16;
        get_minus(valueChar);
        bc_printBigChar(valueChar, offsetCol + 1 , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(val1, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(val2, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(val3, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        offsetCol += 9;
        switchValue(val4, valueChar);
        bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        return;
    } else{

        if (sc_commandDecode(memoryValue, &command, &operand) != 0){
            command = 0;
            operand = 0;
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
            switchValue(command / 16, valueChar);
            bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
            offsetCol += 9;
            switchValue(command % 16, valueChar);
            bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
            offsetCol += 9;
            switchValue(operand / 16, valueChar);
            bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
            offsetCol += 9;
            switchValue(operand % 16, valueChar);
            bc_printBigChar(valueChar, offsetCol , offsetRow + 1, BlackFore, WhiteBack);
        }
        mt_gotoXY(1, 25);
    }
}


