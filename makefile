#
#
#

CC=gcc
CFLAGS=-lX11 -L/usr/lib
DEPS=
OBJS=Xskeleton.o

%.o: %.c $(DEPS)
	$(CC) -c -v -o $@ $< $(CFLAGS)

Xskeleton: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
