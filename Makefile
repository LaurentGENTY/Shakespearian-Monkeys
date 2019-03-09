CFLAGS= -std=c99 -Wall -Wextra# -g3
CC=gcc
OBJ=cell.o queue.o monkey.o utils.o
TESTDIR=tests
EXEC=project
MAKE=make --no-print-directory
RAPPORTDIR=rapport

all: code

code: code-achiev1

code-achiev1: achiev1.c $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $^

cell.o: cell.c cell.h
	$(CC) $(CFLAGS) -o $@ -c $<

queue.o: queue.c queue.h cell.h utils.h
	$(CC) $(CFLAGS) -o $@ -c $<

monkey.o: monkey.c monkey.h cell.h queue.h utils.h
	$(CC) $(CFLAGS) -o $@ -c $<

utils.o: utils.c utils.h cell.h queue.h monkey.h
	$(CC) $(CFLAGS) -o $@ -c $<

test: test-achiev1

test-achiev1: code-achiev1
	(cd $(TESTDIR) && $(MAKE) run-$@)

clean:
	rm -f projet *.o $(RAPPORTDIR)/rapport.*
	(cd $(TESTDIR) && $(MAKE) $@)

report:
	cd $(RAPPORTDIR) && pdflatex rapport.tex

view:
	evince $(RAPPORTDIR)/rapport.pdf
