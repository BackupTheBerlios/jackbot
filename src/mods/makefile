CC = gcc
CFLAGS = -fPIC -ggdb -DDEBUG=$(DEBUG)
MODS_SRCS=$(wildcard *.c)
MODS_OBJS=$(MODS_SRCS:%.c=%.o)
MODS_TRGT=$(MODS_OBJS:%.o=../../bin/mods/%.so)

all: $(MODS_TRGT)

$(MODS_OBJS): $(MODS_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $(@:%.o=%.c)

$(MODS_TRGT): $(MODS_OBJS)
	ld -rpath ../../bin/lib -L../../bin/lib -shared -o $@ $(notdir $(@:%.so=%.o)) -lc -ljackbot
