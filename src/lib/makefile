CC = gcc
CFLAGS = -ggdb -c -Wall -Werror -Wextra -fPIC -DDEBUG=$(DEBUG)
LD = gcc
BOTLIB = ../../bin/lib/libjackbot.so # vorher .a
BOTOBJ = $(notdir $(BOTLIB:.so=.o))

$(BOTLIB): $(BOTOBJ)
#$(LD) -lc -static -soname $(notdir $(BOTLIB)) -o $(BOTLIB) $^
#	ar rcs $(BOTLIB) $(BOTOBJ)
	$(LD) -shared -o $(BOTLIB) $(BOTOBJ)

$(BOTOBJ): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<
