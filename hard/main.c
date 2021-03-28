#include "hard.h"
#include <stdlib.h>
int main(){
    tCHS* chs = malloc(20);
    tLBA* lba = malloc(32);
    tLARGE* large = malloc(24);
    tIDECHS* idechs = malloc(28);
	typedef struct{
		int active;
		tLBA* l_start;
		tCHS* c_start;
		tCHS* finish;
		uint32_t size;
	}Table;
    int h;
    int s;
    int c;
	int sectionSize = 1;
    int choose;
	int freeSpace = 4000;
 
	Table table[128]; //= malloc(sizeof(Table)* 128);
    printf("Input IDECHS geometry\n");
    printf("C: ");
    scanf("%d", &c);
    printf("H: ");
    scanf("%d", &h);
    printf("S: ");
    scanf("%d", &s);
	idechs->c = c;
	idechs->h = h;
	idechs->s = s;
	g_idechs2lba(idechs, lba);
	double a = lba->s * 512;
	a /= 1024;
	a /= 1024;
	a /= 1024;
	printf("Disk size: %lf GB\n", a);
    //printf("Free space(KiB): ");
    //scanf("%d", &freeSpace);
    printf("Тип:\n");
    printf("1 - FAT32 2- Linux swap 3 - HPFS/NTFS\n");
    scanf("%d", &choose);
	printf("Free space: %d\n", freeSpace - 0);
	int i = 0;
	table[i].c_start = malloc(20);
	table[i].finish = malloc(20);
	table[i].l_start = malloc(32);
	table[i].l_start->s = 1;
	g_lb2chs(table[i].l_start, table[i].c_start);

    while ( freeSpace - sectionSize > 0){
	printf("Input section size: ");
    scanf("%d", &sectionSize);
	if (sectionSize == 0)
		break;
	if (freeSpace - sectionSize < 0)
		break;
	table[i].size = sectionSize;
	if (i != 0) {
		table[i].c_start = malloc(20);
		table[i].finish = malloc(20);
		table[i].l_start = malloc(32);
		table[i].l_start->s = table[i - 1].l_start->s + table[i - 1].size;
		g_lb2chs(table[i].l_start, table[i].c_start);
	}
	freeSpace -= sectionSize;
	lba->s = sectionSize + table[i].l_start->s;
	g_lb2chs(lba, table[i].finish);
	printf("Set section as active (1/0):\n");
	scanf("%d", &table[i].active);
	if ((table[i].active == 1) && (i!=0)) {
		for (int j = 0; j < i; j++) {
			table[j].active = 0;
		}
	}
	printf("Free space: %d\n", freeSpace);
	i++;
	sectionSize = 0;
	}
	printf("Active| CHS start | Type | CHS finish | LBA start | Size\n");
	for (int j = 0; j < i; j++) {
		printf("   %d ", table[j].active);
		printf("	  %d %d %d ", table[j].c_start->c, table[j].c_start->h, table[j].c_start->s);
		printf("    %d ", choose);
		printf("       %d %d %d ", table[j].finish->c, table[j].finish->h, table[j].finish->s);
		printf("     %d	     %d \n", table[j].l_start->s, table[j].size);
	}
}