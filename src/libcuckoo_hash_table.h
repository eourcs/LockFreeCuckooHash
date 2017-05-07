#ifndef LIBCUCKOO_HASH_TABLE
#define LIBCUCKOO_HASH_TABLE

#include <libcuckoo/cuckoohash_map.hh>
#include "hash_table.h"

struct Libcuckoo_hash_table {
  Libcuckoo_hash_table(int capacity, int thread_count) : map(capacity) { }

  std::pair<int, bool> search(int key, int tid) {
    try {
      int val = map.find(key);
      return std::make_pair(val, true);
    } catch (std::exception e) {
      return std::make_pair(0, false);  
    }
  }

  void                 insert(int key, int val, int tid) {
    map.insert_or_assign(key, val);
  }

  void                 remove(int key, int tid) {
    map.erase(key);
  }

private:
  cuckoohash_map<int, int> map;
  
};

#endif
