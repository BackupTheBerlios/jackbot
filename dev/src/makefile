CC = gcc
CFLAGS = -ggdb -c -Wall -Werror -Wextra
OBJS = main.o pre.o io.o

#$(OBJS): %.o: %.c
#	$(CC) $(CFLAGS) -o $@ $<

all: $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

pre.o: pre.c
	$(CC) $(CFLAGS) -o $@ $<

io.o: io.c
	$(CC) $(CFLAGS) -o $@ $<
