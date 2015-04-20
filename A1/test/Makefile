# BOOOS Tests Makefile

include ../Makedefs

all: FORCE $(TEST).o
	g++ $(LDFLAGS) $(TEST).o -g -lBOOOS -o $(TEST)

clean:
	rm -f *.o *_Test

FORCE: 
	
