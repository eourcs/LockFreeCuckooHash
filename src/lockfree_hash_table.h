#ifndef LOCKFREE_HASH_TABLE
#define LOCKFREE_HASH_TABLE

#include "hash_table.h"

struct Lockfree_hash_table : Hash_table {

  Lockfree_hash_table(int capacity);
  
  std::pair<int, bool> search(int key);
  void                 insert(int key, int val);
  void                 remove(int key);
};


#endif
