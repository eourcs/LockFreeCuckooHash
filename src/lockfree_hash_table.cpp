#include "lockfree_hash_table.h"
#include <cstdint>

#define THRESHOLD 50

// Inline bit twiddling functions
inline Count_ptr make_pointer(Hash_entry* e, uint16_t count) {
  return (Count_ptr)((((uint64_t)count) << 48) | ((uint64_t)e & 0xFFFFFFFFFFFF));
}
inline Hash_entry* get_pointer(Count_ptr ptr) {
  return (Hash_entry*)((uint64_t)ptr & 0xFFFFFFFFFFFF);
}

inline uint16_t get_counter(Count_ptr ptr) { 
  return (uint16_t)(((uint64_t)ptr >> 48) & 0xFFFF);
}

inline bool get_marked(Hash_entry *ent) {
  return ((uint64_t)ent & 1) == 1;
}

inline Hash_entry *set_marked(Hash_entry *ent, bool marked) {
  return marked ? (Hash_entry*)((uint64_t)ent | 1) 
                : (Hash_entry*)((uint64_t)ent & (~1));
}

Lockfree_hash_table::Lockfree_hash_table(int capacity) {
  size1 = capacity / 2;
  size2 = capacity - size1;

  table[0] = new Count_ptr[size1]();
  table[1] = new Count_ptr[size2]();
}

void rehash() {

}

// Private
int Lockfree_hash_table::hash1(int key) {
  int c2=0x27d4eb2d; // a prime or an odd constant
  key = (key ^ 61) ^ (key >> 16);
  key = key + (key << 3);
  key = key ^ (key >> 4);
  key = key * c2;
  key = key ^ (key >> 15);
  return key % size1;
}

int Lockfree_hash_table::hash2(int key) {
  key = ((key >> 16) ^ key) * 0x45d9f3b;
  key = ((key >> 16) ^ key) * 0x45d9f3b;
  key = (key >> 16) ^ key;
  return key % size2;
}

bool Lockfree_hash_table::check_counter(int ts1, int ts2, int ts1x, int ts2x) {
  return (ts1x >= ts1 + 2) && (ts2x >= ts2 + 2) && (ts2x >= ts1 + 3);
}

Find_result Lockfree_hash_table::find(int key, Count_ptr &e1, Count_ptr &e2) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  Find_result result = (Find_result)-1;

  while (true) {
    e1 = table[0][h1];
    int ts1 = get_counter(e1);

    if (get_pointer(e1)) {
      if (get_marked(e1)) {
        help_relocate(0, h1, false);
        continue; 
      }

      if (get_pointer(e1)->key == key) 
        result = FIRST; 
    }

    e2 = table[1][h2];
    int ts2 = get_counter(e2);

    if (get_pointer(e2)) {
      if (get_marked(e2)) {
        help_relocate(1, h2, false);
        continue; 
      }

      if (get_pointer(e2)->key == key) {
        if (result == FIRST) {
          del_dup(h1, e1, h2, e2);
        } else {
          result = SECOND;
        }
      }
    }

    if (result == FIRST || result == SECOND) {
      return result;
    }

    e1 = table[0][h1];
    e2 = table[1][h2];

    if (check_counter(ts1, ts2, get_counter(e1), get_counter(e2))) {
      continue;
    } else {
      return NIL;
    }
  }
}

bool Lockfree_hash_table::relocate(int which, int index) {
  int  route[THRESHOLD];
  int  start_level = 0;
  int  pre_idx;
  int  idx = index;
  int  tbl = which;

path_discovery:
  bool found = false;
  int  depth = start_level;
  while (!found && depth < THRESHOLD)
  {
    Count_ptr ptr1 = table[tbl][idx];
    Hash_entry* e1 = get_pointer(ptr1);

    while (get_marked(e1))
    {
      help_relocate(tbl, idx, false);
      e1 = get_pointer(table[tbl][idx]);
    }

    if (e1 != nullptr)
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
    for (int i = depth-1; i >= 0; i--, tbl = 1 - tbl)
    {
      idx = route[i];
      Count_ptr ptr1 = table[tbl][idx];
      Hash_entry* e1 = get_pointer(ptr1);

      if (get_marked(e1))
      {
        help_relocate(tbl, idx, false);
        e1 = get_pointer(table[tbl][idx]);
      }

      if (e1 == nullptr)
        continue;

      int dest_idx = (tbl == 0) ? hash2(e1->key) : hash1(e1->key);
      Count_ptr ptr2 = table[1-tbl][dest_idx];
      Hash_entry* e2 = get_pointer(ptr2);

      if (e2 != nullptr)
      {
        start_level = i + 1;
        idx = dest_idx;
        tbl = 1 - tbl;
        goto path_discovery;
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
    Hash_entry* src = get_pointer(ptr1);
    while (initiator && !get_marked(src))
    {
      if (src == nullptr)
        return;

      __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                   set_marked(ptr1, 1));
      ptr1 = table[which][index];
      src  = get_pointer(ptr1);
    }

    if (!get_marked(src))
      return;

    int hd = (1 - which) ? hash1(src->key) : hash2(src->key);
    Count_ptr ptr2  = table[1-which][hd];
    Hash_entry* dst = get_pointer(ptr2);

    uint16_t ts1 = get_counter(ptr1);
    uint16_t ts2 = get_counter(ptr2);

    if (dst == nullptr)
    {
      int nCnt = ts1 > ts2 ? ts1 + 1 : ts2 + 1;
      if (ptr1 != table[which][index])
        continue;
      if (__sync_bool_compare_and_swap(&table[1-which][hd], ptr2, 
                                       make_pointer(src, nCnt)))
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                     make_pointer(nullptr, ts1+1));
        return;
      }

      if (src == dst)
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                     make_pointer(nullptr, ts1+1));
        return;
      }

      __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                   make_pointer(set_marked(src, 0), ts1+1));
      return;
    }
    
  }
}

void Lockfree_hash_table::del_dup(int idx1, Count_ptr ptr1, int idx2, Count_ptr ptr2) {
  if (ptr1 != table[0][idx1] && ptr2 != table[1][idx2])
    return;
  if (get_pointer(ptr1)->key != get_pointer(ptr2)->key)
    return;

  __sync_bool_compare_and_swap(&table[1][idx2], ptr2, 
                               make_pointer(nullptr, get_counter(ptr2)));
}
  
// Public
std::pair<int, bool> Lockfree_hash_table::search(int key) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  while (true) {
    Count_ptr ptr1 = table[0][h1];
    Hash_entry *e1 = get_pointer(ptr1);
    int ts1 = get_counter(e1);

    if (e1 && e1->key == key)
      return std::make_pair(e1->val, true);

    Count_ptr ptr2 = table[1][h2];
    Hash_entry *e2 = get_pointer(ptr2);
    int ts2 = get_counter(e2);

    if (e2 && e2->key == key)
      return std::make_pair(e2->val, true);

    int ts1x = get_counter(table[0][h1]);
    int ts2x = get_counter(table[1][h2]);

    if (check_counter(ts1, ts2, ts1x, ts2x))
      continue;
    else
      return std::make_pair(0, false);
  }

  return std::make_pair(0, false);
}

void Lockfree_hash_table::insert(int key, int val) {
  Count_ptr e1, e2;

  Hash_entry *new_node = new Hash_entry();

  int h1 = hash1(key);
  int h2 = hash2(key);

  while (true) {
    Find_result result = find(key, e1, e2);

    if (result == FIRST) {
      get_pointer(e1)->val = val; 
      return;
    }

    if (result == SECOND) {
      get_pointer(e2)->val = val;
      return;
    }

    if (!get_pointer(e1)) { 
      if (!__sync_bool_compare_and_swap(
            &table[0][h1], e1, make_pointer(new_node, get_counter(e1)))) {
        continue; 
      }
      return;
    }

    if (!get_pointer(e2)) { 
      if (!__sync_bool_compare_and_swap(
            &table[1][h2], e2, make_pointer(new_node, get_counter(e2)))) {
        continue; 
      }
      return;
    }

    if (relocate(0, h1)) {
      continue;
    } else {
      rehash();
    }
  }
}

void Lockfree_hash_table::remove(int key) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  Count_ptr e1;
  Count_ptr e2;

  while (true) {
    Find_result ret = find(key, e1, e2);

    if (ret == NIL) return;

    if (ret == FIRST) {
      if (__sync_bool_compare_and_swap(
            &table[0][h1], e1, make_pointer(nullptr, get_counter(e1)))) {
        return;
      }
    } else if (ret == SECOND) {
      if (table[0][h1] != e1) 
        continue;
      if (__sync_bool_compare_and_swap(
            &table[1][h2], e2, make_pointer(nullptr, get_counter(e2)))) {
        return;
      }
    }
  }
}
