CC=clang
CFLAGS=-std=c11 -g -fno-common

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

TEST_SRCS=$(wildcard test/*.c)
TESTS=$(TEST_SRCS:.c=.exe)

peachcc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): peachcc.h

test/%.exe: peachcc test/%.c
	$(CC) -o- -E -P -C test/$*.c | ./peachcc -o test/$*.s -
	$(CC) -o $@ test/$*.s -x c test/common

test: $(TESTS)
	for i in $^; do echo $$i; ./$$i || exit 1; echo; done
	./test-driver.sh

clean:
	rm -f peachcc tmp* ${TESTS} test/*.s test/*.exe
	find * -type f '(' -name '*~' -o -name '*.o' ')' -exec rm {} ';'


.PHONY: test clean
