# BOOOS Main Makefile

include Makedefs

LIBDIR := lib
TESTDIR := test

all: booos.o
	(cd $(LIBDIR) && make all)
	g++ booos.o $(LDFLAGS) -lBOOOS -o booos

test: FORCE
	(cd $(TESTDIR) && make all)

clean: FORCE
	(cd $(LIBDIR) && make clean)
	(cd $(TESTDIR) && make clean)
	rm -f *.o booos core*

FORCE: 
	
