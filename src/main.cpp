#include <hash_table.h>
#include <cycle_timer.h>

#include <random>
#include <fstream>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS          64
#define DEFAULT_THREAD_COUNT 24
#define DEFAULT_ELEM_COUNT   24000000
#define DEFAULT_READ_PERCENT 90
#define DEFAULT_LOAD_FACTOR  40

#define MAX_ITER             5

// Worker thread arguments
typedef struct
{
  int         elems_per_thread;
  double      rweight;
  double      iweight;
  double      dweight;
  Hash_table *ht_p;
}

void action(void* arg)
{
  WorkerArgs *args = (WorkerArgs *)arg;
  
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> rng;

  std::array<double, 3> weights;
  weights[0] = args->rweight;
  weights[1] = args->iweight;
  weights[2] = args->dweight;
  
  std::default_random_engine gen();
  std::discrete_distribution<int> drng (weights.begin(), weights.end());

  int n = args->elems_per_thread;
  for (int i = 0; i < n; i++)
  {
    int k = rng(mt);
    int v = rng(mt);
    int a = drng(gen);

    // Read
    if (a = 0)
    {
      *ht_p.find(k);
    } 
    // Insert
    else if (a = 1)
    {
      *ht_p.insert(k, v);
    }
    // Delete
    else
    {
      *ht_p.delete(k);
    }
  }
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
  
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> rng;
 
  double start, time;
  std::vector<double> results;
  for (int iter = 0; iter < MAX_ITER; iter++)
  {
    // Allocate table
    Hash_table ht(num_elems); 

    // Warm up table to load factor
    for (int i = 0; i < load_factor * table_size; i++)
    {
      int k = rng(mt);
      int v = rng(mt);
      ht.insert(k, v);
    }

    // Run tests
    int elems_per_thread = num_elems / thread_count;
    pthread_t workers[MAX_THREADS];

    start = CycleTimer::currentSeconds();
    // Launch workers
    for (int i = 0; i < thread_count; i++)
    {
      pthread_t tid;
      WorkerArgs args;
      args.elems_per_thread = elems_per_thread;
      args.rweight = static_cast<double>(read_percent) / 100.0;
      args.iweight = 1.0 - (args.rweight / 2.0);
      args.dweight = 1.0 - (args.rweight / 2.0);
      args.ht_p    = &ht;
      pthread_create(&workers[i], NULL, action, &args);
    }

    // Wait for threads to finish
    for (int i = 0; i < thread_count; i++)
    {
      pthread_join(workers[i], NULL);
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
