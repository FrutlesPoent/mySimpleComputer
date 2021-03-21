#include "hard.h"
#include <stdlib.h>

int main(){
    tCHS* chs = malloc(20);
    tLBA* lba = malloc(32);
    tLARGE* large = malloc(24);
    tIDECHS* idechs = malloc(28);

    int h;
    int s;
    int c;
    printf("Input head number, sector number, celinder number");
    scanf("%d %d %d", &h, &s, &c);
    
    // chs -> h = 1;
    // chs -> s = 1;
    // chs -> c = 1;

    lba -> s = 17408;
    g_lb2chs(lba, chs);
    printf("%d", chs -> h);
    printf("%d", chs -> s);
    printf("%d", chs -> c);

    //g_chs2lba(chs, lba);

    //printf("%d", lba -> s);

}