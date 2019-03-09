// Librairies

#include <stdio.h>

// Files

#include "utils_tests.h"

unsigned int test_count;
unsigned int test_count_wk;

// Log function to display test results
void log_test(int status, char *test_name)
{
  test_count++;
  if(status)
  {
    test_count_wk++;
    printf("%s completed successfully.\n", test_name);
  }
  else
  {
    fprintf(stderr, "%s failed.\n", test_name);
  }
}

// Log a line to show where does the test begin
void start_tests(char *test_name)
{
  printf("---- Beginning %s tests ----\n", test_name);
}

// Log a line to show where does the test end and some stats
void end_tests(char *test_name)
{
  printf("-> %d/%d tests were successful\n", test_count_wk, test_count);
  printf("---- End of %s tests ----\n", test_name);
}
