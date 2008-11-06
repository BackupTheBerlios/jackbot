CC = gcc
CFLAGS = -ggdb -Wall -Werror -Wextra -L../dev/lib/ 
LIBS = -lc -lncurses -ljackbot
OBJS = ../dev/src/main.o ../dev/src/pre.o ../dev/src/io.o
#LIB = ../dev/lib/libjackbot.a

jackboti : 
	$(CC) $(CFLAGS) -o jackbot $(OBJS) $(LIBS)
