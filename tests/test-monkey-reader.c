// Librairies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Files

#include "utils_tests.h"
#include "cell.h"
#include "queue.h"
#include "monkey.h"
#include "utils.h"

/* Variables to play the game differently */
int MAX_TURNS = 30000;
int seed;
char *TEXT;

/* Global variables for handling some informations */
int words_read;
int words_printed;
int sentence_size;
int endReading;

// turns count
int turns_count = 0;
int len = 0;

char test_name[] = "test-monkey-reader";

/*
* Here we test the case where we try to open and read a file.txt.
*/
int test_open_file()
{
  return (init_read());
}

/*
* Here we test the case where we added a new reader monkey, and we check if it
* had been successfully added.
*/
int test_create_reader(struct monkey *monkeys[])
{
  return (monkeys[READER]->role == READER && monkeys[READER]->activity == 1 && monkeys[READER]->queue != NULL);
}

/*
* Here we test the case where the reader monkey tries to read the first word
* of the file (and at the end, we check that he is still active and alive).
*/
int test_work_reader(struct monkey **monkeys,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
   struct pool *pool,
	 struct poolQueues *poolQueues)
{
  turns_count++;
  work(monkeys[READER], queueReader, queueStats, queueWriter, pool, poolQueues);
  len = (monkeys[READER]->activity == 1); // =1

  struct cell *c = pop(queueReader);
  push(queueReader, c);
  return (strcmp(c->word, "a") == 0 && monkeys[READER]->activity == 1);
}

/*
* Here we test the case where the reader monkey tries to read the whole file
* (and at the end, we check that he is now dead).
*/
int test_work_reader_eof(struct monkey **monkeys,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
   struct pool *pool,
	 struct poolQueues *poolQueues)
{
  while(len >=1 && turns_count < MAX_TURNS)
  {
    turns_count++;
    work(monkeys[READER], queueReader, queueStats, queueWriter, pool, poolQueues);
    len = (monkeys[READER]->activity == 1);
  }
  struct cell *c[4];
  for(int i = 0; i < 4; i++)
  {
    c[i] = pop(queueReader);
    push(queueReader, c[i]);
  }
  return (strcmp(c[0]->word, "a") == 0
          && strcmp(c[1]->word, "b") == 0
          && strcmp(c[2]->word, "c") == 0
          && strcmp(c[3]->word, "a") == 0
          && monkeys[0]->activity == -1);
}

int main()
{
  start_tests(test_name);

  // Init
  TEXT = "input.txt"; // contains the following words: a b c a
  words_read = 0;
  words_printed = 0;

  /* Initial pool initialized statically */
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueues = { {}, poolQueues.m };

  int t_open_file = test_open_file();
  log_test(t_open_file, "test_open_file");

  int t_create_reader = 0, t_work_reader = 0, t_work_reader_eof = 0;

  // Can't run other tests if the file opening didn't work
  if(t_open_file)
  {
    // ---------------Initialization of monkeys -----------------------

    /* Monkeys list for the beginning */
    struct monkey *monkeys[MAX_MONKEYS];
    struct queue queueReader;
    queue_init(&queueReader);
    struct monkey reader = {0, 1, &queueReader, NULL};
    monkeys[READER] = &reader;
    struct queue queueStats;
    queue_init(&queueStats);
    struct monkey statistician = {1, 1, &queueStats, NULL};
    monkeys[STATISTICIAN] = &statistician;
    struct queue queueWriter;
    queue_init(&queueWriter);
    struct monkey printer = {2, 0, &queueWriter, NULL};
    monkeys[PRINTER] = &printer;
    struct monkey writer = {3, 0, &queueWriter, NULL};
    monkeys[WRITER] = &writer;

    t_create_reader = test_create_reader(monkeys);
    log_test(t_create_reader, "test_create_reader");

    t_work_reader = test_work_reader(monkeys, &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
    log_test(t_work_reader, "test_work_reader (only one word)");
    
    t_work_reader_eof = test_work_reader_eof(monkeys, &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
    log_test(t_work_reader_eof, "test_work_reader_eof (rest of the file)");
  }

  end_tests(test_name);

  if(t_open_file && t_create_reader && t_work_reader && t_work_reader_eof)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
