CFLAGS=-std=c11 -g -fno-common

9cc: main.o
	$(CC) -o 9cc main.o $(LDFLAGS)
test: 9cc
	./test.sh
clean:
	rm -f 9cc *.o *~ tmp*
.PHONY: test clean
