#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


#ifndef READKEY_H
#define READKEY_H

enum Keys {
    KEY_up,
    KEY_down,
    KEY_right,
    KEY_left,
    KEY_enter,
    KEY_wrong,
    KEY_l,
    KEY_s,
    KEY_t,
    KEY_i,
    KEY_r,
    KEY_f5,
    KEY_f6,
};

int rk_readkey(enum Keys* key);
int rk_mytermsave();
int rk_mytermrestore();
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);


#endif