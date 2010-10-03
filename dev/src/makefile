CC = gcc
CFLAGS = -ggdb -c -Wall -Werror -Wextra
OBJS = main.o pre.o io.o

all: $(OBJS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

#dependencies: ($OBJS:.o=.c)
#  $(CC) -M $^ > $@

#include dependencies
