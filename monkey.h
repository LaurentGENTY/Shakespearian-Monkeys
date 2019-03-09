#ifndef MONKEY_H
#define MONKEY_H

#define MAX_MONKEYS 4
#define WRITER_SLEEPING 100

/* Role for each monkey */
enum role {READER, STATISTICIAN, PRINTER, WRITER, NB_MONKEYS};

/* Structure of monkey */
struct monkey{
  enum role role;
  /* Strike, dead or active */
  int activity;
  /* Its queue : reader, statistician, writer */
  struct queue *queue;

  /* Owns one cell */ 
  struct cell *cell;
};


/* ------------------- Set functions ------------------- */

/* Initialize variables about the text */
int init_read();

/* We read a word from a file and add words in the queue reader */
/* We need the queue AND the pool to create cells */
int read_word(struct monkey *monkey,
	      struct pool *pool,
	      struct poolQueues *poolQueues);

/* Working function of the statistician */
/* Will read a word from the queue reader then push it in its queue without duplication */
int stats(struct monkey *monkey,
	  struct queue *queueReader,
	  struct pool *pool,
	  struct poolQueues *poolQueues);

/* Print the queue of the writer */
int print_queue(struct monkey *monkey);

/* Write random words among cells of the queue */
int writer_job(struct monkey *monkey,
	       struct queue *queueStats,
	       struct pool *pool,
	       struct poolQueues *poolQueues);

/* Working job depending on the type of monkey */
int work(struct monkey *monkey,
	 struct queue *queueReader,
	 struct queue *queueStats,
	 struct queue *queueWriter,
	 struct pool *pool,
	 struct poolQueues *poolQueues);

/* Fill active monkeys array with only active monkeys*/
struct monkey **filter_active_monkeys(struct monkey **monkeys,
				      struct monkey **actives);

/* Check if monkeys cannot be re-activated */
int check_activities(struct monkey **monkeys,
		     struct queue *queueReader,
		     struct queue *queueWriter,
		     int nb_turns);

/* Potentially re-activate the statistician */
int re_activate_statistician(struct monkey *monkey,
			     const struct queue *queueReader);

/* Potentially re-activate the writer */
int re_activate_writer(struct monkey *monkey,
		       int nb_turns);

/* Potentially re-activate the printer */
int re_activate_printer(struct monkey *monkey,
			const struct queue *queueWriter);

/* ------------------- Get functions ------------------- */

/* Count the number of active monkeys in the array */
int monkeys_active_count(struct monkey **monkeys);

/* Return a random active monkey */
struct monkey *random_select(struct monkey **monkeys,
			     int rand);

#endif
