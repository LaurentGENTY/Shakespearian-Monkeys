// Librairies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Files

#include "cell.h"
#include "queue.h"
#include "monkey.h"
#include "utils.h"

/* Variables to play the game differently */
int MAX_TURNS = 1500;
int seed;
char *TEXT;

/* Global variables for handling some informations */
int words_read;
int words_printed;
int sentence_size;
int endReading;

int main(int argc, char *argv[])
{
  /* Initialization of usefull variables */
  parse_opts(argc, argv);

  /* We check if the file given exists */
  if(!init_read())
    {
      exit(EXIT_SUCCESS);
    }

  srand(seed);
  int rand;
  int turns_count = 0;
  words_read = 0;
  words_printed = 0;
  endReading = 0;
  sentence_size = 0;

  /* Pool for cells and queues */

  /* Initial pool initialized statically */
  struct pool pool = { {}, pool.m };
  struct poolQueues poolQueues = { {}, poolQueues.m };

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

  /* Init an array for the active monkeys (to select randomly an active monkey) */
  struct monkey *active_monkeys[MAX_MONKEYS];
  struct monkey *rand_monkey;

  // ----------------------------------------------------------------

  /* Length allow us to know the number of active monkeys */
  /* At first : all monkeys are active */
  int length = monkeys_active_count(monkeys);

  /* ----------------- Beginning of the game loop ----------------- */

  while(length >=1 && (turns_count < MAX_TURNS))
    {
      turns_count++;

      /* We take a random number between 0 and the length of active monkeys */
      rand = give_rand(length);

      /* We filter only active monkeys among all monkeys */
      filter_active_monkeys(monkeys,active_monkeys);

      /* Select a random monkey among active monkeys */
      rand_monkey = random_select(active_monkeys,rand);

      /* The random monkey works */
      work(rand_monkey,&queueReader,&queueStats,&queueWriter,&pool,&poolQueues);

      /* We check if we can activate monkeys again */
      check_activities(monkeys,&queueReader,&queueWriter,turns_count);

      /* We count again the active monkeys */
      length = monkeys_active_count(monkeys);
    }

  /* ----------------- End of the game loop ----------------- */

  /* End message */
  (length < 1)?
    printf("\nNo more active monkeys!\n"):
    printf("\nMax turns reached: end of the game\n");

  /* Initialization of occurrences of the queue stats */
  struct queue maxQueue;
  queue_init(&maxQueue);
  struct queue minQueue;
  queue_init(&minQueue);

  /* Display messages at the end */
  printf("[ %d turns passed ]\n",turns_count);
  printf("Number of words read: %d\n",words_read);
  printf("Number of different words: %d\n",size(&queueStats));
  printf("Number of words printed: %d\n",words_printed);

  /* Show max and min occurences */
  show_min_max(&queueStats,&maxQueue,&minQueue,&pool,&poolQueues);
  
  return EXIT_SUCCESS;
}
