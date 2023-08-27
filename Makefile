CC=clang
CFLAGS=-std=c11 -g -fno-common
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

peachcc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): peachcc.h

test: peachcc
	./test.sh
	./test-driver.sh

clean:
	rm -f peachcc *.o *~ tmp*
.PHONY: test clean
