#ifndef LOCKFREE_HASH_TABLE
#define LOCKFREE_HASH_TABLE

#include "hash_table.h"

struct Hash_entry {
  int key;
  int val;
};

typedef std::pair<Hash_entry*, int> Count_ptr;

struct Lockfree_hash_table : Hash_table {
  Lockfree_hash_table(int capacity);
  
  std::pair<int, bool> search(int key);
  void                 insert(int key, int val);
  void                 remove(int key);

private:
  Count_ptr *table[2];  

  int hash1(int key);
  int hash2(int key);
  int find(int key, Count_ptr &ptr1, Count_ptr &ptr2);
  void relocate(int which, int index);
  void help_relocate(int which, int index, bool initiator);
  void del_dup(int idx1, Count_ptr ptr1, int idx2, Count_ptr ptr2);
};
#endif
