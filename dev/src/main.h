/********************************\
 * project....: JacKBot         *
 * file.......: main.h          *
 * description: header for main *
\********************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#if (DEBUG)
#include <time.h>
#endif

#include "lib/libjackbot.h"
#include "pre.h"
#include "io.h"

int main(int argc, char *argv[]);
void usage(char *prog_name);
void quit(void);

#endif // _MAIN_H_
