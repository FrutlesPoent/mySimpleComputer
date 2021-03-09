#include <stdio.h>
#include "readkey.h"

enum Keys escSeq(){
    char symbol;
    enum Keys retval;

    symbol = getchar();
    if (symbol != 0x5B)
        return KEY_wrong;
    symbol = getchar();

    switch (symbol)
    {
    case 0x41:
        retval = KEY_up;
        return retval;
    case 0x42:
        retval = KEY_down;
        return retval;
    case 0x43:
        retval = KEY_right;
        return retval;
    case 0x44:
        retval = KEY_left;
        return retval;
    case 0x31:
        break;
    default:
        return KEY_wrong;
    }

    symbol = getchar();
    if (symbol == 0x35)
        retval = KEY_f5;
    else if(symbol == 0x37)
        retval = KEY_f6;
    else
        return KEY_wrong;
    symbol = getchar();
    if (symbol != 0x7E)
        return KEY_wrong;
    return retval;
}

int rk_readkey(enum Keys* key){
    char symbol = getchar();

    if (symbol == 0x1B){
        *key = escSeq();
        if (*key == KEY_wrong)
            return -1;
    } else {
        switch (symbol)
        {
        case 'l':
            *key = KEY_l;
            break;
        case 's':
            *key = KEY_s;
            break;
        case 'r':
            *key = KEY_r;
            break;
        case 't':
            *key = KEY_t;
            break;
        case '\n':
            *key = KEY_enter;
            break;
        case 'i':
            *key = KEY_i;
            break;
        
        default:
            *key = KEY_wrong;
            return -1;
        }
    }
    return 0;
}

int rk_mytermsave(){

    struct termios options;
    int fd = open("myterm.bin", O_CREAT | O_RDWR | O_TRUNC);
    
    if (tcgetattr(STDIN_FILENO, &options) != 0)
        return -1;
    if (!write(fd, &options, sizeof(options)))
        return -1;
    return 0;

}

int rk_mytermrestore(){
    struct termios options;
	int fd = open("myterm.bin", O_RDONLY);

	if (!read(fd, &options, sizeof(options)))
		return -1;
	if (!tcsetattr(STDIN_FILENO, TCSAFLUSH, &options))
		return -1;

	return 0;

}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint){
    struct termios options;

	if (tcgetattr(0, &options) != 0)
		return -1;
	if (regime == 1)
		options.c_lflag |= ICANON;
	else if (regime == 0)
		options.c_lflag &= ~ICANON;
	else return -1;

		options.c_cc[VTIME] = vtime;
		options.c_cc[VMIN] = vmin;
		if (echo == 1)
			options.c_lflag |= ECHO;
		else if (echo == 0)
			options.c_lflag &= ~ECHO;
		else return -1;
		if (sigint == 1)
			options.c_lflag |= ISIG;
		else if (sigint == 0)
			options.c_lflag &= ~ISIG;
		else return -1;
	if (!tcsetattr(0, TCSANOW, &options))
		return -1;

	return 0;

}