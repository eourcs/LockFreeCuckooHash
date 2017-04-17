#include "Lockfree_hash_table.h"

Lockfree_hash_table::Lockfree_hash_table(int capacity) {
  size1 = capacity / 2;
  size2 = capacity - size1;

  table[0] = new Count_ptr[size1];
  table[1] = new Count_ptr[size2];
}

// Private
int Lockfree_hash_table::hash1(int key) {
  //TODO
  return 0;
}

int Lockfree_hash_table::hash2(int key) {
  //TODO
  return 0;
}

bool Lockfree_hash_table::check_counter(int ts1, int ts2, int ts1x, int ts2x) {
  //TODO
  return 0;
}

Find_result Lockfree_hash_table::find(int key, Count_ptr &ptr1, Count_ptr &ptr2) {
  //TODO
  return NIL;
}

void Lockfree_hash_table::relocate(int which, int index) {
  //TODO
}

void Lockfree_hash_table::help_relocate(int which, int index, bool initiator) {
  //TODO
}

void Lockfree_hash_table::del_dup(
  int idx1, Count_ptr ptr1, int idx2, Count_ptr ptr2) {
  //TODO
}
  
// Public
std::pair<int, bool> Lockfree_hash_table::search(int key) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  while (true) {
    Count_ptr ptr1 = table[0][h1];
    Hash_entry *e1 = ptr1->first;
    int ts1 = ptr1->second;

    if (e1 != nullptr && e1->key == key)
      return std::make_pair(e1->val, true);

    Count_ptr ptr2 = table[1][h2];
    Hash_entry *e2 = ptr2->first;
    int ts2 = ptr2->second;

    if (e2 != nullptr && e2->key == key)
      return std::make_pair(e2->val, true);

    int ts1x = table[0][h1]->second;
    int ts2x = table[1][h2]->second;

    if (check_counter(ts1, ts2, ts1x, ts2x))
      continue;
    else
      return std::make_pair(0, false);
  }

  return std::make_pair(0, false);
}

void Lockfree_hash_table::insert(int key, int val) {
  //TODO
}

void Lockfree_hash_table::remove(int key) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  while (true) {
    Hash_entry ent1;
    Hash_entry ent2;

    Count_ptr ptr1 = std::make_pair(&ent1, 0);
    Count_ptr ptr2 = std::make_pair(&ent2, 0);

    Find_result ret = find(key, ptr1, ptr2);

    if (ret == NIL) return;

    if (ret == FIRST) 
      if (__sync_bool_compare_and_swap(&table[0][h1], ptr1, ptr2))
  }
}
