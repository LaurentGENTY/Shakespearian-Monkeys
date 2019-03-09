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

char test_name[] = "test-monkey-statistician";

/*
* Here we test the case where we added a new statistician monkey, and we check
* if it had been successfully added.
*/
int test_create_statistician(struct monkey *monkeys[])
{
  return (monkeys[STATISTICIAN]->role == STATISTICIAN && monkeys[STATISTICIAN]->activity == 1 && monkeys[STATISTICIAN]->queue != NULL);
}

/*
* Here we test the case where the statistician monkey tries to read the get the
* first word read by the reader (and at the end, we check that he successfully
* got that word in its queue and that he is not on strike (queueReader isn't
* empty)).
*/
int test_work_statistician(struct monkey **monkeys,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
   struct pool *pool,
	 struct poolQueues *poolQueues)
{
  turns_count++;
  work(monkeys[STATISTICIAN], queueReader, queueStats, queueWriter, pool, poolQueues);
  len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);

  struct cell *c = pop(queueStats);
  push(queueStats, c);
  return (strcmp(c->word, "a") == 0
          && strcmp(queueReader->first->word, "a") != 0
          && monkeys[STATISTICIAN]->activity == 1
          && len == 2);
}

/*
* Here we test the case where the statistician monkey tries to read the get all
* the words read by the reader (and at the end, we check that he successfully
* got these word in its queue and that he is now on strike).
*/
int test_work_statistician_eof(struct monkey **monkeys,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
   struct pool *pool,
	 struct poolQueues *poolQueues)
{
  len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);

  while(len >=1 && turns_count < MAX_TURNS)
  {
    turns_count++;
    work(monkeys[STATISTICIAN], queueReader, queueStats, queueWriter, pool, poolQueues);
    if(!is_empty(queueReader) && !is_first_read(queueReader))
    {
      if(monkeys[STATISTICIAN]->activity == 0)
      {
        monkeys[STATISTICIAN]->activity = 1;
      }
    }
    len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);
  }

  struct cell *c[3];
  for(int i = 0; i < 3; i++)
  {
    c[i] = pop(monkeys[STATISTICIAN]->queue);
    push(queueStats, c[i]);
  }

  return (is_empty(queueReader)
          && strcmp(c[0]->word, "a") == 0
          && c[0]->noccs == 2
          && strcmp(c[1]->word, "b") == 0
          && c[1]->noccs == 1
          && strcmp(c[2]->word, "c") == 0
          && c[2]->noccs == 1
          && monkeys[0]->activity == -1
          && monkeys[1]->activity == 0
          && len == 0);
}

int main()
{
  start_tests(test_name);

  // Init
  TEXT = "input.txt"; // contains the following words: a b c a
  words_read = 0;
  words_printed = 0;

  // Initial pool initialized statically
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueues = { {}, poolQueues.m };

  int t_create_stat = 0, t_work_stat = 0, t_work_stat_eof = 0;

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

    t_create_stat = test_create_statistician(monkeys);
    log_test(t_create_stat, "test_create_statistician");

    for(int i = 0; i < 2; i++)
    {
      turns_count++;
      work(monkeys[READER], &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
      len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);
    }
    t_work_stat = test_work_statistician(monkeys, &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
    log_test(t_work_stat, "test_work_statistician (only one word)");

    while(len >=2 && turns_count < MAX_TURNS)
    {
      turns_count++;
      work(monkeys[READER], &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
      len = (monkeys[READER]->activity == 1)+(monkeys[STATISTICIAN]->activity == 1);
    }
    t_work_stat_eof = test_work_statistician_eof(monkeys, &queueReader, &queueStats, &queueWriter, &pool, &poolQueues);
    log_test(t_work_stat_eof, "test_work_statistician_eof (rest of the file)");
  }

  end_tests(test_name);

  if(t_create_stat && t_work_stat && t_work_stat_eof)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
