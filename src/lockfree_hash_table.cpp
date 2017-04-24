#include "lockfree_hash_table.h"

#define THRESHOLD 50

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
  int  route[THRESHOLD];
  bool found = false;
  int  start_level = 0;

  // Path Discovery
  int idx = index;
  int tbl = which;
  int depth = start_level;
  int pre_idx;
  while (!found && depth < THRESHOLD)
  {
    Count_ptr ptr1 = table[tbl][idx];
    Hash_entry* e1 = ptr1->first;
    while (e1 & 0x1)
    {
      help_relocate(tbl, idx, false)
      e1 = table[tbl][idx]->first;
    }

    if (e1 != NULL)
    {
      route[depth] = idx;
      int key = e1->key;
      pre_idx = idx;
      tbl     = 1 - tbl;
      idx     = (tbl == 0) ? hash1(key) : hash2(key); 
    }
    else
    {
      found = true;
    }
     
    depth++;
  }

  if (found)
  {
    tbl = 1 - tbl;
    for (int i = depth-1; i >= 0; i--, tbl = 1 - tbl;)
    {
      idx = route[i];
      Count_ptr ptr1 = table[tbl][idx];
      Hash_entry* e1 = ptr1->first;
      if (e1 & 0x1)
      {
        help_relocate(tbl, idx, false);
        e1 = table[tbl][idx]->first;
      }

      if (e1 == NULL)
        continue;

      int dest_idx = (tbl == 0) ? hash2(e1->key) : hash1(e1->key);
      Count_ptr ptr2 = table[1-tbl][dest_idx];

      if (e2 != NULL)
      {
        start_level = i + 1;
        idx = dest_idx;
        tbl = 1 - tbl;
        //goto path_discovery;
      }
      help_relocate(tbl, idx, false);
    }
  }

  return found;
}

void Lockfree_hash_table::help_relocate(int which, int index, bool initiator) {
  while (1)
  {
    Count_ptr ptr1  = table[which][index];
    Hash_entry* src = ptr1->first;
    while (initiator && !(src & 0x1))
    {
      if (src == NULL)
        return;

      __sync_bool_compare_and_swap(&table[which][index], ptr1, ptr1 | 0x1);
      ptr1 = table[which][index];
      src  = ptr1->first;
    }

    if (!(src & 0x1))
      return;

    int hd = (1 - which) ? hash1(src->key) : hash2(src->key);
    Count_ptr ptr2  = table[1-which][hd];
    Hash_entry* dst = ptr2->first;
    if (dst == NULL)
    {
      int nCnt = ptr1->second > ptr2->second ? ptr1->second+1 : ptr2->second+1;
      if (ptr1 != table[which][index])
        continue;
      if (__sync_bool_compare_and_swap(&table[1-which][hd], ptr2, std::make_pair()))
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, );
        return;
      }

      if (src == dst)
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, );
        return;
      }

      __sync_bool_compare_and_swap(&table[which][index], ptr1, );
      return false;
    }
    
  }
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
