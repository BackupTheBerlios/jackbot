CC = gcc
CFLAGS = -ggdb -c -Wall -Werror -Wextra
LD = ld
BOTLIB = ../../lib/libjackbot.a
BOTOBJ = $(notdir $(BOTLIB:.a=.o))

$(BOTLIB): $(BOTOBJ)
#$(LD) -lc -static -soname $(notdir $(BOTLIB)) -o $(BOTLIB) $^
	ar rcs $(BOTLIB) $(BOTOBJ)

$(BOTOBJ): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<
