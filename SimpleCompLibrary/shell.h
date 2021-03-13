//#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include "myBigChars.h"
#include "terminal.h"
#include "register.h"
#include "memory.h"
#include "readkey.h"
#include <signal.h>
#include <sys/time.h>

#ifndef SHELL_H
#define SHELL_H

static int positionRowShell = 0;
static int positionColShell = 0;

int shellRun();

void set_color(ForegroundColors, BackgroundColors);
void memory_paint();
void memory_index_paint(int index);
void accumulator_paint();
void instruction_counter_paint();
void operation_paint();
void flags_paint();
void keys_paint();
void box_paint();
void flagsRegPaint();

#endif