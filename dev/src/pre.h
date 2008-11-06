/*************************************\
 * project....: JacKBot              *
 * file.......: pre.h                *
 * desrciption: prototypes for pre.c *
\*************************************/
#ifndef _PRE_H_
#define _PRE_H_

#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include "lib/libjackbot.h"
#include "io.h"
#include "main.h"

#define BREAK "\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\n"

inline void init(void);
inline void do_connect(void);
inline void load_mods(void);

#endif // _PRE_H_
