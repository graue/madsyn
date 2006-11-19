.PHONY: all clean

EXENAME = madsyn
INSTALLDIR = $(HOME)/bin/

CFLAGS = -W -Wall -Werror -O2 -ggdb
LDFLAGS = -lm
OBJS = op.o xm.o err.o

all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CC) -o $(EXENAME) $(LDFLAGS) $(OBJS)

xm.o: xm.c err.h
err.o: err.c
op.o: op.c op.h defs.h

clean:
	rm -f *.o $(EXENAME)

install: $(EXENAME)
	cp $(EXENAME) $(INSTALLDIR)
