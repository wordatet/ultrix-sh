# Makefile for Ultrix 4.2 Bourne Shell port to Linux
# Compatible with both GNU make and bmake

CROSS_COMPILE ?=
CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=gnu89 -O0 -g -I. -include compat.h \
         -Wno-implicit-int -Wno-implicit-function-declaration -Wno-return-type \
         -Wno-incompatible-pointer-types -Wno-int-conversion \
         -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast

OBJS = args.o blok.o builtin.o cmd.o ctype.o error.o expand.o \
       fault.o io.o macro.o main.o msg.o name.o print.o service.o \
       setbrk.o stak.o string.o word.o xec.o

all: sh

sh: $(OBJS)
	$(CC) $(CFLAGS) -o sh $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) sh

# Dependencies
blok.o: blok.c defs.h mode.h mac.h brkincr.h
fault.o: fault.c defs.h brkincr.h
main.o: main.c defs.h brkincr.h
stak.o: stak.c defs.h brkincr.h stak.h
args.o: args.c defs.h
builtin.o: builtin.c defs.h
cmd.o: cmd.c defs.h sym.h
ctype.o: ctype.c defs.h ctype.h
error.o: error.c defs.h
expand.o: expand.c defs.h
io.o: io.c defs.h
macro.o: macro.c defs.h sym.h
msg.o: msg.c defs.h
name.o: name.c defs.h
print.o: print.c defs.h
service.o: service.c defs.h
setbrk.o: setbrk.c defs.h
string.o: string.c defs.h
word.o: word.c defs.h sym.h
xec.o: xec.c defs.h sym.h

.PHONY: all clean
