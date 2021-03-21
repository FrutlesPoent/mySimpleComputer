#include "hard.h"
#include <math.h>
#define CHS_HEAD 4
#define CHS_CELINDER 10
#define LARGE_HEAD 8
#define LARGE_CELINDER 10
#define IDECHS_HEAD 4
#define IDECHS_CELINDER 16



void g_lb2chs(tLBA* lba, tCHS* chs){
   int tmp1 =  pow(2, CHS_HEAD);
   int tmp2 = pow(2, CHS_CELINDER);

   chs -> c = (int) (lba -> s / (pow(2, CHS_HEAD) * pow(2, CHS_CELINDER)));
   chs -> h = (int) (lba -> s / pow(2, CHS_CELINDER)) % tmp1;
   chs -> s = (int) (lba -> s % tmp2 + 1);
}

void g_chs2lba(tCHS* chs, tLBA* lba) {

   lba -> s = ((chs -> c * pow(2, CHS_HEAD) + chs -> h) * pow(2, CHS_CELINDER)) + chs -> s - 1;
  
}

void g_lb2large(tLBA* lba, tLARGE* large){
   int tmp1 =  pow(2, LARGE_HEAD);
   int tmp2 = pow(2, LARGE_CELINDER);

   large -> c = (int) (lba -> s / tmp1 * tmp2);
   large -> h = (int) (lba -> s / tmp2) % tmp1;
   large -> s = (int) (lba -> s % tmp2 + 1);
}

void g_large2lba (tLARGE* large, tLBA* lba){
   int tmp1 =  pow(2, LARGE_HEAD);
   int tmp2 = pow(2, LARGE_CELINDER); 

   lba -> s = ((large -> c * tmp1 + large -> h) * tmp2) + large -> s - 1;

}

void g_lba2idechs(tLBA* lba, tIDECHS* idechs){
   int tmp1 =  pow(2, IDECHS_HEAD);
   int tmp2 = pow(2, IDECHS_CELINDER);

   idechs -> c = (int) (lba -> s / tmp1 * tmp2);
   idechs -> h = (int) (lba -> s / tmp2) % tmp1;
   idechs -> s = (int) (lba -> s % tmp2 + 1);
}

void g_idechs2lba (tIDECHS* idechs, tLBA* lba){
   int tmp1 =  pow(2, IDECHS_HEAD);
   int tmp2 = pow(2, IDECHS_CELINDER);

   lba -> s = ((idechs -> c * tmp1 + idechs -> h) * tmp2) + idechs -> s - 1;
}

void g_chs2large (tCHS* chs, tLARGE* large) {

   uint32_t tmp = ((chs -> c * pow(2, CHS_HEAD) + chs -> h) * pow(2, CHS_CELINDER)) + chs -> s - 1;

   int tmp1 = pow(2, LARGE_HEAD);
   int tmp2 = pow(2, LARGE_CELINDER);

   large -> c = (int) (tmp / tmp1 * tmp2);
   large -> h = (int) (tmp / tmp2) % tmp1;
   large -> s = (int) (tmp % tmp2 + 1);
}

void g_large2chs (tLARGE* large, tCHS* chs){
   uint32_t tmp = ((large -> c * pow(2, LARGE_HEAD) + large -> h) * pow(2, LARGE_CELINDER)) + large -> s - 1;

   int tmp1 =  pow(2, CHS_HEAD);
   int tmp2 = pow(2, CHS_CELINDER);

   chs -> c = (int) (tmp / tmp1 * tmp2);
   chs -> h = (int) (tmp / tmp2) % tmp1;
   chs -> s = (int) (tmp % tmp2 + 1);

}

void g_chs2idechs (tCHS* chs, tIDECHS* idechs) {

   uint32_t tmp = ((chs -> c * pow(2, CHS_HEAD) + chs -> h) * pow(2, CHS_CELINDER)) + chs -> s - 1;

   int tmp1 =  pow(2, IDECHS_HEAD);
   int tmp2 = pow(2, IDECHS_CELINDER);

   idechs -> c = (int) (tmp / tmp1 * tmp2);
   idechs -> h = (int) (tmp / tmp2) % tmp1;
   idechs -> s = (int) (tmp % tmp2 + 1);

}

void g_large2idechs (tLARGE* large, tIDECHS* idechs){
   uint32_t tmp = ((large -> c * pow(2, LARGE_HEAD) + large -> h) * pow(2, LARGE_CELINDER)) + large -> s - 1;

   int tmp1 =  pow(2, IDECHS_HEAD);
   int tmp2 = pow(2, IDECHS_CELINDER);

   idechs -> c = (int) (tmp / tmp1 * tmp2);
   idechs -> h = (int) (tmp / tmp2) % tmp1;
   idechs -> s = (int) (tmp % tmp2 + 1);
}

void g_idechs2chs (tIDECHS* idechs, tCHS* chs) {

   uint32_t tmp = ((idechs -> c * pow(2, IDECHS_HEAD) + idechs -> h) * pow(2, IDECHS_CELINDER)) + idechs -> s - 1;

   int tmp1 =  pow(2, CHS_HEAD);
   int tmp2 = pow(2, CHS_CELINDER);

   chs -> c = (int) (tmp / tmp1 * tmp2);
   chs -> h = (int) (tmp / tmp2) % tmp1;
   chs -> s = (int) (tmp % tmp2 + 1);
}

void g_idechs2lagre (tIDECHS* idechs, tLARGE* large){
   uint32_t tmp = ((idechs -> c * pow(2, IDECHS_HEAD) + idechs -> h) * pow(2, IDECHS_CELINDER)) + idechs -> s - 1;
   int tmp1 = pow(2, LARGE_HEAD);
   int tmp2 = pow(2, LARGE_CELINDER);

   large -> c = (int) (tmp / tmp1 * tmp2);
   large -> h = (int) (tmp / tmp2) % tmp1;
   large -> s = (int) (tmp % tmp2 + 1);
}

