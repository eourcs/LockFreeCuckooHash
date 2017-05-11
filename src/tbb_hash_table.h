#ifndef TBB_HASH_TABLE
#define TBB_HASH_TABLE

#include "hash_table.h"
#include <tbb/concurrent_hash_map.h>

using namespace tbb;

struct Hash {
  static size_t hash(const int& i) {
    int c2=0x27d4eb2d; // a prime or an odd constant
    int key = i;
    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * c2;
    key = key ^ (key >> 15);
    return key;
  }

  static bool equal(const int& x, const int& y) {
    return x == y;
  }
};

typedef concurrent_hash_map<int, int, Hash> Table;

struct TBB_hash_table {
  TBB_hash_table(int capacity, int thread_count) { }

  std::pair<int, bool> search(int key, int tid) {
    Table::const_accessor a;

    if (tbl.find(a, key)) {
      return std::make_pair(a->first, true);
    } else {
      return std::make_pair(0, false);
    }
  }

  void                 insert(int key, int val, int tid) {
    Table::accessor a;
    tbl.insert(a, key);
    a->second = val;
  }

  void                 remove(int key, int tid) {
    tbl.erase(key);
  }

private:
  Table tbl; 
};

#endif
