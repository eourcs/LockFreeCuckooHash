#include <hash_table.h>
#include <cycle_timer.h>

#include <random>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define DEFAULT_THREAD_COUNT 24
#define DEFAULT_ELEM_COUNT   20000000
#define DEFAULT_READ_PERCENT 90
#define DEFAULT_LOAD_FACTOR  40

#define MAX_ITER             5

// PRNG setup
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> rng;

void action(void* arg)
{
  // TODO: Weighted PRNG?
  return;  
}

int main(int argc, char *argv[])
{
  int c;
  int num_elements = DEFAULT_ELEM_COUNT; 
  int num_threads  = DEFAULT_THREAD_COUNT;
  int read_percent = DEFAULT_READ_PERCENT;
  int load_factor  = DEFAULT_LOAD_FACTOR;

  char *out_file   = NULL;

  // Parse command line arguments
  while ((c = getopt(argc, argv, "nto:rhl")) != -1)
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
      case 'l':
        load_factor = *optarg;
        break;
      case 'h':
        printf("Options: \n"
               "-n num_elements \n"
               "-t num_threads \n"
               "-l load_factor \n"
               "-r read_percent \n"
               "-o output_file \n");
        break;
      default:
        break;
    }
  }
 
  double start, time;
  std::vector<double> results;
  for (int iter = 0; iter < MAX_ITER; iter++)
  {
    // Allocate table
    

    // Warm up table to load factor
    for (int i = 0; i < load_factor * table_size; i++)
    {
      int k = rng(mt);
      int v = rng(mt);
      ht.insert(k, v);
    }

    //Run tests
    start = CycleTimer::currentSeconds();
    for (int i = 0; i < num_elems; i++)
    {
      // TODO: fill in
    }
    time  = CycleTimer::currentSeconds() - start;

    // Log results
    printf("%d: %f\n", iter, time);
    results.push_back(time);
  }

  double avg_time = 0;
  for (auto& t : results)
    avg_time += t;
  avg_time /= static_cast<double>(results.size());

  // TODO: Format output
  std::ofstream o_fd(out_file, std::ios_base::app);
  
}
