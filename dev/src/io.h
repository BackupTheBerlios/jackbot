/********************************************\
 * project....: JacKBot                     *
 * file.......: io.h                        *
 * description: prototypes for input/output *
\********************************************/
#ifndef _IO_H_
#define _IO_H_

#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/libjackbot.h"

// define colors
#define CLR_ERROR 1
#define CLR_MSG   2
#define CLR_OK    3
#define CLR_HEAD  4

// define macros
#define printf_color(COLOR,...) sprintf(buffer_, __VA_ARGS__);\
        print_color(COLOR, buffer_);

void print_color(short int pair, const char* string);
void parse_msg(char *msg);

#endif // _IO_H_
