LIBNAME=libbaralho
BINFOLDER=../bin/
LIBFOLDER=$(HOME)/lib/
INCFOLDER=$(HOME)/include/
LIBFILE=$(LIBNAME).a
INCFILES=baralho.h
HEADER=baralho.h
OBJECTS=baralho.o
TESTFILE=test.c

CC=gcc -Wall -g
AR=ar -rcu
INSTALL=install

.PHONY: clean distclean all test

%.o: %.c $(INCFILES)
	$(CC) -c $<

all: $(LIBFILE)

test: $(LIBFILE) $(TESTFILE)
	$(CC) $(TESTFILE) -o $(BINFOLDER)test -L$(LIBFOLDER) -lbaralho

$(LIBFILE): $(OBJECTS)
	$(AR) $(LIBFILE) $(OBJECTS)
	ranlib $(LIBFILE)

install: $(LIBFILE)
	$(INSTALL) -m=640 $(INCFILES) $(INCFOLDER)
	$(INSTALL) -m=640 $(LIBFILE) $(LIBFOLDER)

clean:
	@rm -f *~ *% *.swp;

distclean: clean
	@rm -rf *.o $(LIBNAME).*;
