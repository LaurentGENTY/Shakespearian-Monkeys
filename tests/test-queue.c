// Librairies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Files

#include "utils_tests.h"
#include "cell.h"
#include "queue.h"
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

char test_name[] = "test-queue";

/*
* Here we test the case where we create a pool and a new empty queue.
* We return 1 if it is successful: the first element in the queue is NULL, the
* last element is NULL and there are 0 cells in the queue.
*/
int test_queue_empty()
{
  words_read = 0;
  words_printed = 0;

  // Create a pool
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueue = { {}, poolQueue.m };

  // Create a new queue
  struct queue queue;
  queue_init(&queue);

  return (queue.first == NULL && queue.last == NULL && queue.cell_count == 0);
}

/*
* Here we test the case where we create a pool, then a new cell in this pool
* and we return 1 if it is successful: the cell isn't NULL and the word in that
* cell is the word we set at its creation.
*/
int test_new_cell()
{
  words_read = 0;
  words_printed = 0;

  // Create a pool
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueue = { {}, poolQueue.m };

  // Create a new cell
  struct cell *cell1 = create_new_cell(&pool, "test-new-cell", 1, NULL, &poolQueue);

  return (cell1 != NULL && strcmp(cell1->word, "test-new-cell") == 0);
}

/*
* Here we test the case where we create a pool, then a new cell in this pool
* and a new empty queue.
* If we can push the cell in the queue we return 1.
* (the push function returns 0)
*/
int test_push()
{
  words_read = 0;
  words_printed = 0;

  // Create a pool
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueue = { {}, poolQueue.m };

  // Create a new queue
  struct queue queue;
  queue_init(&queue);

  struct cell *cell1 = create_new_cell(&pool, "test-new-cell", 1, NULL, &poolQueue);

  return (push(&queue, cell1));
}

/*
* Here we test the case where we create a pool, then a new cell in this pool
* and a new empty queue.
* We push the cell in the queue, then if we can pop that cell it is successful,
* we return 1.
*/
int test_pop()
{
  words_read = 0;
  words_printed = 0;

  // Create a pool
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueue = { {}, poolQueue.m };

  // Create a new queue
  struct queue queue;
  queue_init(&queue);

  // Create a new cell and push it in the queue
  struct cell *cell1 = create_new_cell(&pool, "test-new-cell", 1, NULL, &poolQueue);
  push(&queue, cell1);

  return (pop(&queue) != NULL);
}

int main()
{
  start_tests(test_name);

  int t_queue_empty = test_queue_empty();
  log_test(t_queue_empty, "test_queue_empty");

  int t_new_cell = test_new_cell();
  log_test(t_new_cell, "test_new_cell");

  int t_push = test_push();
  log_test(t_push, "test_push");

  int t_pop = test_pop();
  log_test(t_pop, "test_pop");

  end_tests(test_name);

  if(t_queue_empty && t_new_cell && t_push && t_pop)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
