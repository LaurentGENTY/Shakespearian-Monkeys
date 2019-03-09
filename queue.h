#ifndef QUEUE_H
#define QUEUE_H

/* Maximum of numbers queues */
#define MAX_QUEUES 20000

/* Necessary structures for the queue */
struct cell;
struct pool;

/* Queue structure */
struct queue {
  struct cell *first;  // will delete this cell index
  struct cell *last;  // will append at index
  int cell_count;
};

/* Pool for the queue */
struct poolQueues {
  struct queue m[MAX_QUEUES];
  struct queue* next_free;
};

/* ------------------- Set functions ------------------- */

/* Create a new queue for a new cell */
struct queue *create_new_queue(struct poolQueues *poolQueues,
			       struct cell *first,
			       struct cell *last);

/* Function to initialize a queue */
void queue_init(struct queue *queue);

/* Empty a queue */
void empty_queue(struct queue *queue);

/* Push a cell in a queue */
int push(struct queue *queue,
	 struct cell *cell);

/* Push a cell in a queue witout duplication */
int push_uniq(struct queue *queue,
	      struct cell *cell);

/* Pop the first cell of a queue */
struct cell* pop(struct queue *queue);

/* Fill minimal and maximal occurrences at the end */
int show_min_max(struct queue *queueStats,
		 struct queue *queueMax,
		 struct queue *queueMin,
		 struct pool *pool,
		 struct poolQueues *poolQueues);

/* ------------------- Get functions ------------------- */

/* Return the size of the queue */
int size(const struct queue *queue);

/* Check if a queue is empty */
int is_empty(const struct queue *queue);

/* Check if the first cell of a queue is read */
int is_first_read(const struct queue *queue);

/* Return the first cell of a queue */
struct cell *first(const struct queue *queue);

/* Checks if a word exists in a queue : returns it if we find it, otherwise NULL  */
struct cell *check(const struct queue *queue,
		   const struct cell *c);

/* Return a random cell in the queue */
struct cell *random_cell(const struct queue *queue);

/* Return a random cell but we take care about the number of occurrences */
struct cell *random_cell_with_occs(const struct queue *queue);

/* ------------------- Debug functions ------------------- */

void show_queue(const struct queue *queue);


#endif
