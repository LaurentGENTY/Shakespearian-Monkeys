// Librairies

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>

// Files

#include "cell.h"
#include "queue.h"
#include "monkey.h"
#include "utils.h"

/* ------------------- Set functions ------------------- */

/* Function for parsing the options of the program */
/* Currently available options are : */
/* -s <seed> : sets the seed */
/* -t <number of turns> : set the number of turns */
/* filename : sets the file to read */
void parse_opts(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt (argc, argv, "s:t:")) != -1) {
    switch (opt)
      {
      case 's':
        seed = atoi(optarg);
        break;
      case 't':
        MAX_TURNS = atoi(optarg);
	break;
      default: /* '?' */
        usage(argv);
    }
  }
  TEXT = argv[optind];
  if (TEXT == NULL)
  {
    usage(argv);
  }
}

/* Print the usage of the game */
void usage(char* argv[])
{
  printf("Usage : %s file [-s seed] [-t number of turns]\n", argv[0]);
  printf("You can switch every options order\n");
  exit(EXIT_SUCCESS);
}

/* ------------------- Get functions ------------------- */

/* Give a random between 0 and max (excluded) */
int give_rand(int max)
{
  return rand()%max;
}

/* Return a random char */
char *random_char()
{
  char *ponctuation[] = {",",
			 ".",
			 ";",
			 "...\n",
			 ":"};
  int r = give_rand(5);

  return ponctuation[r];
}
