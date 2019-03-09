CFLAGS=-I../ -std=c99 -Wall -Wextra# -g3
CC=gcc
OBJTESTQUEUE=../utils.o ../cell.o ../queue.o utils_tests.o
OBJTESTIO=../utils.o ../cell.o ../queue.o ../monkey.o utils_tests.o
TESTS=test-queue test-io test-monkey-reader test-monkey-statistician test-monkey-writer test-monkey-printer

all: test-achievement1

utils_tests.o: utils_tests.c utils_tests.h
	$(CC) $(CFLAGS) -o $@ -c $<

run-test-achiev1: $(TESTS)
	./test-queue
	./test-io
	./test-monkey-reader
	./test-monkey-statistician
	./test-monkey-writer
	./test-monkey-printer

test-queue: test-queue.c $(OBJTESTQUEUE)
	$(CC) $(CFLAGS) -o $@ $^

test-io: test-io.c $(OBJTESTIO)
	$(CC) $(CFLAGS) -o $@ $^

test-monkey-reader: test-monkey-reader.c $(OBJTESTIO)
	$(CC) $(CFLAGS) -o $@ $^

test-monkey-statistician: test-monkey-statistician.c $(OBJTESTIO)
	$(CC) $(CFLAGS) -o $@ $^

test-monkey-writer: test-monkey-writer.c $(OBJTESTIO)
	$(CC) $(CFLAGS) -o $@ $^

test-monkey-printer: test-monkey-printer.c $(OBJTESTIO)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TESTS) *.o
