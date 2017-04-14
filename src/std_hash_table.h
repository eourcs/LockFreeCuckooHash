#ifndef STD_HASH_TABLE
#define STD_HASH_TABLE

#include "hash_table.h"
#include <unordered_map>

struct Std_hash_table : Hash_table {
  std::unordered_map<int, int> map;

  Std_hash_table(int capacity);

  std::pair<int, bool> search(int key);
  void                 insert(int key, int val);
  void                 remove(int key);
};

#endif
