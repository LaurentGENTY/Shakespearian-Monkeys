// Libraries

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// Files

#include "cell.h"
#include "queue.h"
#include "monkey.h"
#include "utils.h"

/* File pointer in ordern to open and read "file_name" */
FILE *fp;

/* External variables */
extern int words_read;
extern int words_printed;
extern int endReading;
extern int sentence_size;

/* ------------------- Set functions ------------------- */

/* Initialize variables about the text */
int init_read()
{
  /* Open the file */
  fp = fopen(TEXT, "r");

  /* If the file doesnt exist, we leave */
  if (fp == NULL)
    {
      perror("Error while opening the file");
      return 0;
    }

  /* The file exists */
  return 1;
}

/* We read a word from a file and add words in the queue reader */
/* We need the queue AND the pool to create cells */
int read_word(struct monkey *monkey,
	      struct pool *pool,
	      struct poolQueues *poolQueues)
{
  assert(monkey->queue);

  /* char[] for words of the file : words musn't excess 20 char */
  char word[MAX_WORD_LENGTH+1];

  /* If we arrived at the end of the text : reader dies */
  if(fscanf(fp, " %20s",word) != 1)
    {
      monkey->activity = -1;
      endReading = 1;
      fclose(fp);
      return 0;
    }

  /* Creating a new cell with the word read */
  struct cell *c = create_new_cell(pool,
				   word,
				   1,
				   NULL,
				   poolQueues);

  /* The reader monkey will push the word in a queue */

  /* We cannot read another words */
  if(c == NULL)
    {
      /* Reader dies because we cannot add new cells in the pool */
      monkey->activity = -1;
      fclose(fp);
      return 0;
    }

  /* We get the return code of the push */
  int code = push(monkey->queue,c);

  if(code == 0)
    {
      /* Reader dies because we cannot add new cells in the pool (too many) */
      monkey->activity = -1;
      fclose(fp);
      return 0;
    }
  else
    {
      /* We could read the word */
      words_read++;
      return 1;
    }
}

/* Working function of the statistician */
/* Will read a word from the queue reader then push it in its queue without duplication */
int stats(struct monkey *monkey,
	  struct queue *queueReader,
	  struct pool *pool,
	  struct poolQueues *poolQueues)
{
  assert(queueReader);
  assert(monkey->queue);

  /* The queue is either empty or the statistician alredy read the first cell */
  if( is_empty(queueReader) || is_first_read(queueReader))
    {
      /* Strike */
      monkey->activity = 0;
      return 0;
    }

  /* If the queue is not empty and the first cell is not already read by stats */
  struct cell *first = queueReader->first;
  struct cell *next  = first->next;

  /* Let's wait for next round to get a successor if next is null */
  if(first->next == NULL && !endReading)
    {
      /* Strike */
      monkey->activity = 0;
      return 0;
    }

  /* Extract the cell from the reader and push it into statistician */
  first = pop(queueReader);

  /* Push it in its queue */
  int code = push_uniq(monkey->queue,first);

  if(code == 0)
    {
      /* Pool is full we cannot create any other words */

      /* Die */
      monkey->activity = -1;
      return 0;
    }

  /* We managed to push the cell */

  /* Has a next */
  if(next != NULL)
    {
      /* We push a copy of the word next in successors queue */
      struct cell *copy = create_new_cell(pool,
					  next->word,
					  next->noccs,
					  NULL,
					  poolQueues);
      push_uniq(first->queue,copy);
    }
  return 1;
}

/* Print the queue of the writer */
int print_queue(struct monkey *monkey)
{
  /* Verify that the queue writer is not null */
  assert(monkey->queue);

  /* If the writer's queue is empty */
  if(is_empty(monkey->queue))
    {
      /* Strike */
      monkey->activity = 0;
      return 0;
    }

  /* We can read the word */
  struct cell *c = pop(monkey->queue);

  /* Print it */
  printf(" %s",c->word);

  words_printed++;

  /* Strike */
  monkey->activity = 0;
  return 1;
}

/* Write random words among cells of the queue */
int writer_job(struct monkey *monkey,
	       struct queue *queueStats,
	       struct pool *pool,
	       struct poolQueues *poolQueues)
{
  assert(queueStats);

  /* If the statistician hasn't read anything yet, I have nothing to do */
  if(is_empty(queueStats))
    {
      /* Strike */
      monkey->activity = 0;
      return 0;
    }

  /* I haven't started a sentence yet */
  if(monkey->cell == NULL)
    {
      /* We choose a random cell in the words already read by the statistician */
      monkey->cell = random_cell(queueStats);

      struct cell *copy = create_new_cell(pool,
					  monkey->cell->word,
					  monkey->cell->noccs,
					  NULL,
					  poolQueues);

      /* We push the new first cell in the queue writer */
      sentence_size = 1;
      push(monkey->queue,copy);
      return 1;
    }

  /* Verify that the monkey->cell is not null */
  assert(monkey->cell != NULL);

  /* We will read a random cell and push it in the queue writer */
  struct cell *next;

  /* If we picked the 10% or the queue of the cell is empty */
  if(is_empty(monkey->cell->queue) || give_rand(10) == 0)
    {
      char *c;

      if(sentence_size > 1)
	{
	  c = random_char();
	}
      else
	{
	  c = "!";
	}

      next = create_new_cell(pool,
			     c,
			     1,
			     NULL,
			     poolQueues);

      push(monkey->queue, next);

      /* We reset the sentence */
      monkey->cell = NULL;
      sentence_size = 0;

      return 1;
    }

  /* Otherwise, we can produce a sentence */

  /* We choose a word randomly taking care of number of occurrences */
  struct cell *random = random_cell_with_occs(monkey->cell->queue);

  /* We found the random cell in the queue */
  assert(random);

  /* Copy the information in the random cell we will push */
  next = create_new_cell(pool,
			 random->word,
			 1,
			 NULL,
			 poolQueues);

  if(next == NULL)
    {
      return -1;
    }

  push(monkey->queue,next);

  /* We find the next cell */
  monkey->cell = check(queueStats, next);

  sentence_size++;

  return 1;
}

/* Working job depending on the type of monkey */
int work(struct monkey *monkey,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
	 struct pool *pool,
	 struct poolQueues *poolQueues)
{
  assert(monkey);
  assert(monkey->queue);
  assert(queueReader);
  assert(queueStats);
  assert(queueWriter);

  if(monkey->activity != 1)
    {
      return 0;
    }

  switch(monkey->role)
    {
    case READER:
      /* Read words in a text */
      return read_word(monkey,pool,poolQueues);
      break;

    case STATISTICIAN:
      /* Will push in the monkey->queue (queue stats) words and pop them */
      return stats(monkey,queueReader,pool,poolQueues);
      break;

    case PRINTER:
      /* Will read a sentence in the queueWriter */
      return print_queue(monkey);
      break;

    case WRITER:
      /* will create sentences with random cell of the queueStats */
      return writer_job(monkey,queueStats,pool,poolQueues);
      break;

    default:
      return 0;
      break;
    }
}

/* Fill active monkeys array with only active monkeys*/
struct monkey **filter_active_monkeys(struct monkey **monkeys,
				      struct monkey **actives)
{
  assert(monkeys);
  assert(actives);

  int k = 0;

  for(int i = 0; i < MAX_MONKEYS; i++)
    {
      /* If the monkey is active we can add it to the array */
      if(monkeys[i]->activity == 1)
	{
	  actives[k] = monkeys[i];
	  k++;
	}
    }
  return actives;
}

/* Check if monkeys cannot be re-activated */
int check_activities(struct monkey **monkeys,
		     struct queue *queueReader,
		     struct queue *queueWriter,
		     int nb_turns)
{
  assert(monkeys);
  assert(queueReader);
  assert(queueWriter);

  /* We cannot re-activate the reader */
  re_activate_statistician(monkeys[STATISTICIAN],queueReader);
  re_activate_writer(monkeys[WRITER],nb_turns);
  re_activate_printer(monkeys[PRINTER],queueWriter);

  return 1;
}

/* Potentially re-activate the statistician */
int re_activate_statistician(struct monkey *monkey,
			     const struct queue *queueReader)
{
  assert(monkey);
  assert(queueReader);

  if(is_empty(queueReader) || is_first_read(queueReader) || endReading)
    {
      return 0;
    }

  /* We can re-activate the statistician if the queueReader is not empty and first word wasnt read*/
  monkey->activity = 1;
  return 1;
}

/* Potentially re-activate the writer */
int re_activate_writer(struct monkey *monkey,
		       int nb_turns)
{
  assert(monkey);

  if(nb_turns < WRITER_SLEEPING)
    {
      return 0;
    }

  /* We can activate the writer */
  monkey->activity = 1;
  return 1;
}

/* Potentially re-activate the printer */
int re_activate_printer(struct monkey *monkey,
			const struct queue *queueWriter)
{
  assert(monkey);
  assert(queueWriter);

  if(is_empty(queueWriter))
    {
      return 0;
    }

  /* We can re-activate the printer if the queue of the writer is not empty*/
  monkey->activity = 1;
  return 1;
}
/* ------------------- Get functions ------------------- */

/* Count the number of active monkeys in the array */
int monkeys_active_count(struct monkey **monkeys)
{
  assert(monkeys);

  int n = 0;
  if(monkeys != NULL)
    {
      //foreach monkeys count the number of actives monkeys
      for(int i = 0; i < MAX_MONKEYS; i++)
	{
	  if(monkeys[i]->activity == 1)
	    {
	      n++;
	    }
	}
    }
  return n;
}

/* Return a random active monkey */
struct monkey *random_select(struct monkey **monkeys,
			     int rand)
{
  assert(monkeys);

  return monkeys[rand];
}
