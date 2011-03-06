CC = gcc
CFLAGS = -ggdb -Wall -Werror -Wextra 
LDFLAGS += -Wl,-rpath=lib/ -Llib/
LIBS = -lc -ljackbot -ldl
OBJS = ../src/main.o ../src/pre.o ../src/io.o
#LIB = ../dev/lib/libjackbot.a

jackboti : 
	$(CC) $(LDFLAGS) $(CFLAGS) -o jackbot $(OBJS) $(LIBS)
