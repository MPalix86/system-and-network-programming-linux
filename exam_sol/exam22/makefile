SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) -I../include -Wall -DMACOS -D_DARWIN_C_SOURCE  -o $@ $< -L../../lib -lapue;
	
#include this flie in examsol folder
