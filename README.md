# Shakespearian Monkeys

A **random text generator in C**: a team of "monkeys" reads an existing text, builds word statistics, and writes new pseudo-Shakespearian text from them.

> School project, ENSEIRB-MATMECA (1st semester), by Laurent Genty and Florian Mornet. Report: [`rapport_shakespearia_monkeys.pdf`](rapport_shakespearia_monkeys.pdf) (French).

## How it works

Each monkey has one job, and they run turn by turn:

- **Reader**: reads words from the input file into a shared queue.
- **Statistician**: counts word occurrences.
- **Writer**: picks words from the queue to build sentences.
- **Printer**: prints the sentences and the final statistics.

Everything is built on hand-written **linked lists and queues** in C99.

## Example output

```
the riper should ; from fairest . from fairest creatures we desire increase that thereby beauty's rose might never die ...
Max turns reached: end of the game
[ 1500 turns passed ]
Number of words read: 83
Number of different words: 65
Number of words printed: 645
Minimal occs : 1
from(1) |
Maximal occs : 5
thy(5) |
```

## Getting started

Requirements: `gcc` (C99), `make`.

```bash
make
./project input/sonnet.txt -s 42 -t 300   # -s: random seed, -t: number of turns
```

Any text file works as input. Keep `-t` reasonable: memory use grows with the number of turns.

## Tests

```bash
make test
```

Unit tests cover the queue, I/O and each monkey (reader, writer, printer, statistician).

## Status

`master` contains achievement 1 (fully working). Achievement 2 (monkeys waking up at sentence ends, one text per monkey) was started on other branches but not finished.
