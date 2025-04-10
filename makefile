CC      = gcc
CFLAGS  = 
RM      = rm


default: all

all: main.c
	$(CC) $(CFLAGS) -o DDEESS main.c KeySchedule.c RoundFunc.c