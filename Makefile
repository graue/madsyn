.PHONY: all clean

EXENAME = madsyn
INSTALLDIR = $(HOME)/bin/

CFLAGS = -W -Wall -O2 -ggdb
LDFLAGS = -lm
OBJS = main.o mutate.o treeio.o play.o op.o random.o xm.o err.o math.o

all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CC) -o $(EXENAME) $(OBJS) $(LDFLAGS)

xm.o: xm.c err.h
err.o: err.c
random.o: random.c
op.o: op.c data.h math.h xm.h
play.o: play.c types.h data.h math.h
treeio.o: treeio.c data.h xm.h err.h
mutate.o: mutate.c data.h xm.h protos.h
main.o: main.c err.h binary.h types.h data.h protos.h
math.o: math.c math.h types.h

clean:
	rm -f *.o $(EXENAME)

install: $(EXENAME)
	cp $(EXENAME) $(INSTALLDIR)
