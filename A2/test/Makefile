# BOOOS Tests Makefile

include ../Makedefs

all: FORCE $(TEST).o
	g++ $(LDFLAGS) $(TEST).o -lBOOOS -o $(TEST)

clean:
	rm -f *.o *_Test

FORCE: 
	
