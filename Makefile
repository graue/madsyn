.PHONY: all clean

EXENAME = madsyn
INSTALLDIR = $(HOME)/bin/

CFLAGS = -W -Wall -Werror -O2 -ggdb -DDEBUG
LDFLAGS = -lm
OBJS = main.o mutate.o treeio.o play.o op.o random.o xm.o err.o

all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CC) -o $(EXENAME) $(LDFLAGS) $(OBJS)

xm.o: xm.c err.h
err.o: err.c
random.o: random.c
op.o: op.c data.h
play.o: play.c types.h data.h
treeio.o: treeio.c data.h xm.h err.h
mutate.o: mutate.c data.h xm.h protos.h
main.o: main.c err.h binary.h types.h data.h protos.h

clean:
	rm -f *.o $(EXENAME)

install: $(EXENAME)
	cp $(EXENAME) $(INSTALLDIR)
