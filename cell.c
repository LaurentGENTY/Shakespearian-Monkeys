// Librairies

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Files

#include "queue.h"
#include "cell.h"

/* ------------------- Set functions ------------------- */

/* Given a pool, and the constituents of a cell, return a pointer to a valid cell with the same constituents or NULL if all the memory has been allocated */

struct cell* create_new_cell(struct pool *pool,
			     char word[],
			     int noccs,
			     struct cell* next,
			     struct poolQueues *poolQueues)
{
  assert(pool);
  
  if(pool->next_free - pool->m >= MAX_CELLS)
    {
      return NULL;
    }

  struct cell* res = pool->next_free;

  /* Always take the null-terminator into account */
  strncpy(res->word, word, MAX_WORD_LENGTH+1);

  res->noccs = noccs;

  res->next = next;

  /* The cell is not read by stats at the beginning */
  res->is_read = 0;

  struct queue *q = create_new_queue(poolQueues,
				     NULL,
				     NULL);
  res->queue = q;
  
  pool->next_free++;  

  return res;
}

/* ------------------- Get functions ------------------- */

/* Given a cell, compute the length of the chained list starting from this cell */
int length(const struct cell* c)
{
  int res = 0;

  while (c!= NULL) {
    c = c->next;
    res++;
  }

  return res;
}
