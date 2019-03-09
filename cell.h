#ifndef CELL_H
#define CELL_H

/* Maximum numbers of cells and maximum word length */
#define MAX_CELLS 20000
#define MAX_WORD_LENGTH 20

/* Necessary structures for the cell */
struct poolQueues;
struct queue;

/* Cell structure */
struct cell {
  char word[MAX_WORD_LENGTH+1];
  int noccs;
  struct cell* next;
  int is_read;
  struct queue *queue;
};

/* Pool in order to allocate statically memory */
struct pool {
  struct cell m[MAX_CELLS];      // an array of memory cells
  struct cell *next_free;        // a pointer to the first free cell
};

/* ------------------- Set functions ------------------- */

struct cell* create_new_cell(struct pool *pool,
			     char word[],
			     int noccs,
			     struct cell* next,
			     struct poolQueues *poolQueues);

/* ------------------- Get functions ------------------- */

int length(const struct cell *c);

#endif
