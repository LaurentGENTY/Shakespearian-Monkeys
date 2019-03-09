
#ifndef UTILS_H
#define UTILS_H

/* External variables that we can change with options */
extern char *TEXT;
extern int seed;
extern int MAX_TURNS;

/* ------------------- Set functions ------------------- */

void parse_opts(int argc, char* argv[]);

/* ------------------- Get functions ------------------- */

/* Print the usage of the game */
void usage();

/* Give a random between 0 and max (excluded) */
int give_rand(int max);

/* Return a random char */
char *random_char();

#endif
