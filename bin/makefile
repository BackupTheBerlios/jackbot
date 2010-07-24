CC = gcc
CFLAGS = -ggdb -Wall -Werror -Wextra 
LDFLAGS += -Wl,-rpath=lib/ -Llib/
LIBS = -lc -lncurses -ljackbot
OBJS = ../dev/src/main.o ../dev/src/pre.o ../dev/src/io.o
#LIB = ../dev/lib/libjackbot.a

jackboti : 
	$(CC) $(LDFLAGS) $(CFLAGS) -o jackbot $(OBJS) $(LIBS)
