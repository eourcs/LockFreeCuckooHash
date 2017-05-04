#ifndef THREAD_SERVICE
#define THREAD_SERVICE

#include <random>
#include <array>

struct WorkerArgs 
{
  int    num_elems;
  // R/I/D weights, normalized to 100
  int    rweight;
  int    iweight;
  int    dweight; 
  void*  ht_p;

  int    start;
  int*   elems;
};

template<typename T>
void* thread_service(void* threadArgs)
{
  WorkerArgs* args = static_cast<WorkerArgs*>(threadArgs);

  std::random_device                 rd;
  std::mt19937                       mt(rd());
  std::uniform_int_distribution<int> rng;

  std::array<int, 3> weights;
  weights[0] = args->rweight;
  weights[1] = args->iweight;
  weights[2] = args->dweight;

  std::default_random_engine         g;
  std::discrete_distribution<int>    drng(weights.begin(), weights.end());

  int num_elems = args->num_elems;
  T* ht_p = static_cast<T*>(args->ht_p);

  for (int i = 0; i < num_elems; i++)
  {
    // Key, Value pair
    int k = rng(mt);
    int v = rng(mt);
    // Action : 0 -> Search, 1 -> Insert, 2 -> Remove
    int a = drng(g);

    if (a == 0)
      ht_p->search(k);
    else if (a == 1)
      ht_p->insert(k, v);
    else
      ht_p->remove(k);
  }
}

template<typename T>
void* thread_insert(void* threadArgs)
{
  WorkerArgs* args = static_cast<WorkerArgs*>(threadArgs);
  int* elems = args->elems;
  T*   ht_p  = static_cast<T*>(args->ht_p);
  int  start     = args->start;
  int  num_elems = args->num_elems;

  for (int i = start; i < start + num_elems; i++)
  {
    ht_p->insert(elems[i], elems[i]);
    std::pair<int, bool> r = ht_p->search(elems[i]);
  }
  
}

#endif
