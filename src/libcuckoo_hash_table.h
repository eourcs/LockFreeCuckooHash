#ifndef LIBCUCKOO_HASH_TABLE
#define LIBCUCKOO_HASH_TABLE

#include "hash_table.h"

struct Libcuckoo_hash_table {
  Libcuckoo_hash_table(int capacity, int thread_count) { }

  std::pair<int, bool> search(int key, int tid) {
    return std::make_pair(0, false);
  }

  void                 insert(int key, int val, int tid) {
  }

  void                 remove(int key, int tid) {
  }

private:
  
};

#endif
