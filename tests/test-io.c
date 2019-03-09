// Librairies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

// Files

#include "utils_tests.h"
#include "utils.h"
#include "cell.h"
#include "monkey.h"

extern int optind;

/* Variables to play the game differently */
int MAX_TURNS = 30000;
int seed;
char *TEXT;

/* Global variables for handling some informations */
int words_read;
int words_printed;
int sentence_size;
int endReading;

char test_name[] = "test-io";

// Function for parsing the options of the program
// Currently available options are :
// -s <seed> : sets the seed
// filename : sets the file to read
void parse_opts_no_exit(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt (argc, argv, "s:")) != -1) {
    switch (opt)
    {
      case 's':
        seed = atoi(optarg);
        break;
      default: /* '?' */
        printf("Usage : %s [file] [-s seed]\n", argv[0]);
    }
  }
  TEXT = argv[optind];
  if (TEXT == NULL)
  {
    printf("Usage : %s [file] [-s seed]\n", argv[0]);
  }
}

/*
* Here we test the case where we run the program with a seed and a filename as
* argument (./project -s 42 file.txt or ./project file.txt -s 42).
*/
int test_parse_seed(int argc, char* argv[])
{
  optind = 1; // to reset getopt
  parse_opts_no_exit(argc, argv);
  return (TEXT != NULL && seed == 42 && strcmp(TEXT, "file.txt") == 0);
}

/*
* Here we test the case where we run the program with only a filename as
* argument (./project file.txt).
*/
int test_parse(int argc, char* argv[])
{
  optind = 1; // to reset getopt
  parse_opts_no_exit(argc, argv);
  return (TEXT != NULL && strcmp(TEXT, "file.txt") == 0);
}

/*
* Here we test the case where we run the program without args (./project).
*/
int test_parse_no_args(int argc, char* argv[])
{
  optind = 1; // to reset getopt
  parse_opts_no_exit(argc, argv);
  return (TEXT == NULL);
}

/*
* Here we test the case where we try to create an empty file and then to open
* and read it (and then delete that file).
*/
int test_init_read()
{
  TEXT = "file.txt";
  FILE *fp = fopen(TEXT, "w");
  fclose(fp);
  int ret = init_read();
  remove(TEXT);
  return ret;
}

/*
* Here we test the case where we try to open and read an unexisting file.
*/
int test_init_read_not_exist()
{
  return (init_read() == 0);
}

int main()
{
  start_tests(test_name);

  char* argv1[] = {"./project", "-s", "42", "file.txt", NULL};
  int t_parse1 = test_parse_seed(4, argv1);
  log_test(t_parse1, "test_parse_seed with \"./project -s 42 file.txt\"");

  char* argv2[] = {"./project", "file.txt", "-s", "42", NULL};
  int t_parse2 = test_parse_seed(4, argv2);
  log_test(t_parse2, "test_parse_seed with \"./project file.txt -s 42\"");

  char* argv3[] = {"./project", "file.txt", NULL};
  int t_parse3 = test_parse(2, argv3);
  log_test(t_parse3, "test_parse with \"./project file.txt\"");

  char* argv4[] = {"./project", NULL};
  int t_parse4 = test_parse_no_args(1, argv4);
  log_test(t_parse4, "test_parse_no_args with \"./project\"");

  int t_init_read = test_init_read();
  log_test(t_init_read, "test_init_read with an existing, empty \"file.txt\"");

  int t_init_read_not_exist = test_init_read_not_exist();
  log_test(t_init_read_not_exist, "test_init_read_not_exist with an unexisting file");

  end_tests(test_name);

  if(t_parse1 && t_parse2 && t_parse3 && t_parse4 && t_init_read)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
