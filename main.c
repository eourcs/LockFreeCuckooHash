//#include <hash_table.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_THREAD_COUNT 24
#define DEFAULT_ELEM_COUNT   20000000
#define DEFAULT_READ_PERCENT 90

int main(int argc, char *argv[])
{
  int c;
  int num_elements = DEFAULT_ELEM_COUNT; 
  int num_threads  = DEFAULT_THREAD_COUNT;
  int read_percent = DEFAULT_READ_PERCENT;

  char *out_file   = NULL;

  // Parse command line arguments
  while ((c = getopt(argc, argv, "nto:rh")) != -1)
  {
    switch (c)
    {
      case 'n':
        num_elements = *optarg;
        break;
      case 't':
        num_threads = *optarg;
        break;
      case 'o':
        out_file = optarg;
        break;
      case 'r':
        read_percent = *optarg;
        break;
      case 'h':
        printf("Options: \n"
               "-n num_elements \n"
               "-t num_threads \n"
               "-r read_percent \n"
               "-o output_file \n");
        break;
      default:
        break;
    }
  }

  // Run test framework

}
