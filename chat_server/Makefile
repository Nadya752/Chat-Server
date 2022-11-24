CC=gcc
CFLAGS=-Wall -Werror -Wvla -std=gnu11 -fsanitize=address
PFLAGS=-fprofile-arcs -ftest-coverage
DFLAGS=-g
HEADERS=server.h
SRC=server.c

procchat: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $@

test: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(PFLAGS) $(SRC) -o $@
	bash test.sh
	
clean:
	rm -f procchat
	rm -f test
	find . -type p -delete
	rm -f gevent
