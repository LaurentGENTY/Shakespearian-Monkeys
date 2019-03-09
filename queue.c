// Librairies

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Files

#include "cell.h"
#include "queue.h"
#include "utils.h"

/* ------------------- Set functions ------------------- */

/* Create a new queue for a new cell */
struct queue *create_new_queue(struct poolQueues *poolQueues,
			       struct cell *first,
			       struct cell *last)
{
  /* Verify that the pool for queues was initialized before */
  assert(poolQueues);
  
  if(poolQueues->next_free - poolQueues->m >= MAX_QUEUES)
    {
      return NULL;
    }

  struct queue *q = poolQueues->next_free;

  q->first = first;
  q->last = last;
  q->cell_count = 0;

  poolQueues->next_free++;
  
  return q;
}

/* Function to initialize a queue */
void queue_init(struct queue *queue)
{
  assert(queue);
  
  /* Initialize each variables of the structure */
  queue->first = NULL;
  queue->last = NULL;
  queue->cell_count = 0;
}

/* Empty a queue */
void empty_queue(struct queue *queue)
{
  /* Verify that the queue is not null and exists */
  assert(queue);
  
  while (!is_empty(queue))
    {
      pop(queue);
    }
}

/* Push a cell in a queue */
int push(struct queue *queue,
	 struct cell  *cell)
{
  assert(queue);

  /* The given cell doesnt exist */
  if(cell == NULL)
    {
      return 0;
    }

  cell->next = NULL;

  /* If the queue is empty we add at the beginning */
  if(is_empty(queue))
    {
      queue->first = cell;
    }
  else
    {
      /* We append */
      assert(queue->last != cell);
      queue->last->next = cell;
    }

  queue->last = cell;
  queue->cell_count++;
  return 1;
}

/* Push a cell in a queue witout duplication */
int push_uniq(struct queue *queue,
	      struct cell  *cell)
{
  assert(queue);
  
  if(cell == NULL)
    {
      return 0;
    }

  if(!is_empty(queue))
    {
      /* We check if the cell already exists or not */
      struct cell *exists = check(queue, cell);

      /* If it exists : we increase the number of occurrences of this word */
      if(exists != NULL)
	{
	  exists->noccs++;
	  return 2;
	}
    }

  /* Then the queue is empty just push it */
  return push(queue,cell);
}

/* Pop the first cell of a queue */
struct cell* pop(struct queue *queue)
{
  assert(queue);

  /* If its empty */
  if(is_empty(queue))
    { 
      return NULL;
    }
  
  struct cell* cell = first(queue);

  queue->first = queue->first->next;

  queue->cell_count--;

  cell->next = NULL;
  return cell;

}

/* Fill minimal and maximal occurrences at the end */
int show_min_max(struct queue *queueStats,
		 struct queue *queueMax,
		 struct queue *queueMin,
		 struct pool *pool,
		 struct poolQueues *poolQueues)
{
  assert(queueStats);
  
  if(is_empty(queueStats))
    {
      return 0;
    }

  int min = first(queueStats)->noccs;
  int max = min;

  struct cell *it = first(queueStats);

  struct cell *c = create_new_cell(pool,
				   it->word,
				   it->noccs,
				   NULL,
				   poolQueues);
  push(queueMin,c);
  push(queueMax,c);

  it = it->next;

  while(it)
    {
      /* Create a copy of the cell */
      struct cell *copy = create_new_cell(pool,
					  it->word,
					  it->noccs,
					  NULL,
					  poolQueues);
      if(!copy)
	{
	  return 0;
	}
      
      /* Minimum case */
      if(copy->noccs < min)
	{
	  /* We can empty the minimum queue and start it again */
	  empty_queue(queueMin);

	  min = copy->noccs;
	  
	  push(queueMin,copy);
	}
      else if(copy->noccs == min)
	{
	  push(queueMin,copy);
	}

      /* Maximum case */
      if(copy->noccs > max)
	{
	  empty_queue(queueMax);

	  max = copy->noccs;
	  
	  push(queueMax,copy);
	}
      else if(copy->noccs == max)
	{
	  push(queueMax,copy);
	}

      it = it->next;
    }

  /* Print the queues */
  printf("Minimal occs : %d\n",min);
  show_queue(queueMin);
  printf("Maximal occs : %d\n",max);
  show_queue(queueMax);
  return 1;
}

/* ------------------- Get functions ------------------- */

/* Return the size of the queue */
int size(const struct queue *queue)
{
  assert(queue);
  return queue->cell_count;
}

/* Check if a queue is empty */
int is_empty(const struct queue *queue) {
  assert(queue);
  return queue->cell_count == 0;
}

/* Check if the first cell of a queue is read */
int is_first_read(const struct queue *queue)
{
  assert(queue);
  return (first(queue)->is_read == 1);
}

/* Return the first cell of a queue */
struct cell* first(const struct queue *queue)
{
  assert(queue);
  return queue->first;
}

/* Checks if a word exists in a queue : returns it if we find it, otherwise NULL  */
struct cell *check(const struct queue *queue, const struct cell *c)
{
  assert(queue);

  struct cell *it = first(queue);
  while(it != NULL)
    {
      /* We compare the words */
      if(strcmp(it->word, c->word)==0)
	{
	  /* We find the word */
	  return it;
	}
      it = it->next;
    }

  return NULL;
}

/* Return a random cell in the queue */
struct cell *random_cell(const struct queue *queue)
{
  assert(queue);

  if(size(queue) == 0)
    {
      return NULL;
    }

  int r = give_rand(size(queue));
  int i = 0;
  
  struct cell *it = first(queue);
  i++;

  while(i < r)
    {
      it = it->next;
      i++;
    }

  return it;
}

/* Return a random cell but we take care about the number of occurrences */
struct cell *random_cell_with_occs(const struct queue *queue)
{
  assert(queue);

  if(is_empty(queue))
    {
      return NULL;
    }

  int total = 0;
  struct cell *it = first(queue);

  while(it)
    {
      total =+ it->noccs;
      it = it->next;
    }

  /* The total of words is total and we choose a random within it */
  int r = give_rand(total);

  /* We go back at the beginning */
  it = first(queue);
  
  while((it != NULL) && (r >= it->noccs))
    {
      r = r - it->noccs;
      it = it->next;
    }

  return it;
}

/* ------------------- Debug functions ------------------- */

/* Print a queue */
void show_queue(const struct queue *queue)
{
  assert(queue);

  if(!is_empty(queue))
    {
      struct cell *c = first(queue);
      while(c != NULL)
	{
	  printf("%s(%d) | ",c->word,c->noccs);
	  c = c->next;
	}
      printf("\n");
    }
}
