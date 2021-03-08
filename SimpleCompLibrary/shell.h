#include <stdio.h>
#include "myBigChars.h"
#include "terminal.h"
#include "register.h"
#include "memory.h"

#ifndef SHELL_H
#define SHELL_H

static int positionRowShell = 0;
static int positionColShell = 0;

void shellRun();

void set_color(ForegroundColors, BackgroundColors);
void memory_paint();
void memory_index_paint(int index);
void accumulator_paint();
void instruction_counter_paint();
void operation_paint();
void flags_paint();
void keys_paint();
void box_paint();

#endif