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

char test_name[] = "test-monkey-printer";

/*
* Here we test the case where we added a new printer monkey, and we check if it
* had been successfully added.
*/
int test_create_printer(struct monkey *monkeys[])
{
  return (monkeys[PRINTER]->role == PRINTER && monkeys[PRINTER]->activity == 0 && monkeys[PRINTER]->queue != NULL && monkeys[PRINTER]->queue == monkeys[WRITER]->queue);
}

/*
* Here we test the case where the printer monkey tries to get the first word in
* the writer queue and print it (and at the end, we check that he is still
* active and alive).
*/
int test_work_printer(struct monkey **monkeys,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
   struct pool *pool,
	 struct poolQueues *poolQueues)
{
  turns_count = 100;
  check_activities(monkeys, queueReader, queueWriter, turns_count);

  // Need to make the writer monkey work in order to fill its queue
  work(monkeys[WRITER], queueReader, queueStats, queueWriter, pool, poolQueues);
  check_activities(monkeys, queueReader, queueWriter, turns_count);

  work(monkeys[PRINTER], queueReader, queueStats, queueWriter, pool, poolQueues);
  printf("\n");

  return (words_printed == 1
          && is_empty(monkeys[PRINTER]->queue)
          && monkeys[PRINTER]->activity == 0
          && len == 2);
}

int main()
{
  start_tests(test_name);

  // Init
  TEXT = "input.txt"; // contains the following words: a b c a
  words_read = 0;
  words_printed = 0;

  // Initial pool initialized writerically
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueues = { {}, poolQueues.m };

  int t_create_printer = 0, t_work_printer = 0;

  // Can't run other tests if the file opening didn't work
  if(init_read())
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

    t_create_printer = test_create_printer(monkeys);
    log_test(t_create_printer, "test_create_printer");

    for(int i = 0; i < 2; i++)
    {
      turns_count++;
      work(monkeys[READER], &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
      len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);
    }
    turns_count++;
    work(monkeys[STATISTICIAN], &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
    len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);
    t_work_printer = test_work_printer(monkeys, &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
    log_test(t_work_printer, "test_work_printer (only one word)");

    while(len >=2 && turns_count < MAX_TURNS)
    {
      turns_count++;
      work(monkeys[READER], &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
      len = monkeys_active_count(monkeys);
    }
    while(len >=1 && turns_count < MAX_TURNS)
    {
      turns_count++;
      work(monkeys[STATISTICIAN], &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
      if(!is_empty(&queueReader) && !is_first_read(&queueReader))
      {
        if(monkeys[STATISTICIAN]->activity == 0)
        {
          monkeys[STATISTICIAN]->activity = 1;
        }
      }
      len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);
    }
  }

  end_tests(test_name);

  if(t_create_printer && t_work_printer)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
