#ifndef LOCKFREE_HASH_TABLE
#define LOCKFREE_HASH_TABLE

#include "hash_table.h"

struct Hash_entry {
  int key;
  int val;
};

// Alternate count_ptr definition using unused bits
typedef Hash_entry* Count_ptr;

enum Find_result { FIRST, SECOND, NIL };

struct Lockfree_hash_table {
  Lockfree_hash_table(int capacity, int thread_count);
  ~Lockfree_hash_table();
  
  std::pair<int, bool> search(int key, int tid);
  void                 insert(int key, int val, int tid);
  void                 remove(int key, int tid);

private:
  Count_ptr *table[2];  
  int size1;
  int size2;

  int hash1(int key);
  int hash2(int key);
  bool check_counter(int ts1, int ts2, int ts1x, int ts2x);
  Find_result find(int key, Count_ptr &ptr1, Count_ptr &ptr2, int tid);
  bool relocate(int which, int index, int tid);
  void help_relocate(int which, int index, bool initiator, int tid);
  void del_dup(int idx1, Count_ptr ptr1, int idx2, Count_ptr ptr2, int tid);
};
#endif
