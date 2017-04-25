#include "lockfree_hash_table.h"
#include <cstdint>

#define THRESHOLD 50

// Inline bit twiddling functions
inline Count_ptr make_pointer(Hash_entry* e, uint16_t count) {
  return (Count_ptr)((((uint64_t)count) << 48) | (uint64_t)e);
}
inline Hash_entry* get_pointer(Count_ptr ptr) {
  return (Hash_entry*)((uint64_t)ptr & 0x00FFFFFE);
}

inline uint16_t get_counter(Count_ptr ptr) { 
  return (uint16_t)(((size_t)ptr >> 48) & 0xFFFF);
}

inline Count_ptr set_counter(Count_ptr ptr, uint16_t counter) {
  return (Count_ptr)((size_t)ptr | (size_t)counter);
}

inline bool get_marked(Hash_entry *ent) {
  return ((size_t)ent & 1) == 1;
}

inline Hash_entry *set_marked(Hash_entry *ent, bool marked) {
  return marked ? (Hash_entry*)((size_t)ent | 1) 
                : (Hash_entry*)((size_t)ent & (~1));
}

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
  return (ts1x >= ts1 + 2) && (ts2x >= ts2 + 2) && (ts2x >= ts1 + 3);
}

Find_result Lockfree_hash_table::find(int key, Count_ptr &ptr1, Count_ptr &ptr2) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  Find_result result;

  while (true) {
    ptr1 = table[0][h1];
    Hash_entry *e1 = ptr1->first;
    int ts1 = ptr1->second;

    if (e1) {
      if (IS_MARKED(e1)) {
        help_relocate(0, h1, false);
        continue; 
      }

      if (e1->key == key) {
        result = FIRST; 
      }
    }

    ptr2 = table[1][h2];
    Hash_entry *e2 = ptr2->first;
    int ts2 = ptr2->second;

    if (e2) {
      if (IS_MARKED(e2)) {
        help_relocate(1, h2, false);
        continue; 
      }

      if (e2->key == key) {
        if (result == FIRST) {
          del_dup(h1, ptr1, h2, ptr2);
        } else {
          result = SECOND;
        }
      }
    }

    if (result == FIRST || result == SECOND) {
      return result;
    }

    ptr1 = table[0][h1];
    ptr2 = table[1][h2];

    if (check_counter(ts1, ts2, ptr1->second, ptr2->second)) {
      continue;
    } else {
      return NIL;
    }
  }
}

bool Lockfree_hash_table::relocate(int which, int index) {
  int  route[THRESHOLD];
  bool found = false;
  int  start_level = 0;

  // Path Discovery
path_discovery:
  int idx = index;
  int tbl = which;
  int depth = start_level;
  int pre_idx;
  while (!found && depth < THRESHOLD)
  {
    Count_ptr ptr1 = table[tbl][idx];
    Hash_entry* e1 = get_pointer(ptr1);

    while (get_marked(e1))
    {
      help_relocate(tbl, idx, false);
      e1 = get_pointer(table[tbl][idx]);
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

      if (e1 == NULL)
        continue;

      int dest_idx = (tbl == 0) ? hash2(e1->key) : hash1(e1->key);
      Count_ptr ptr2 = table[1-tbl][dest_idx];
      Hash_entry* e2 = get_pointer(ptr2);

      if (e2 != NULL)
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
      if (src == NULL)
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

    if (dst == NULL)
    {
      int nCnt = ts1 > ts2 ? ts1 + 1 : ts2 + 1;
      if (ptr1 != table[which][index])
        continue;
      if (__sync_bool_compare_and_swap(&table[1-which][hd], ptr2, 
                                       make_pointer(src, nCnt)))
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                     make_pointer(NULL, ts1+1));
        return;
      }

      if (src == dst)
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                     make_pointer(NULL, ts1+1));
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
                               make_pointer(NULL, get_counter(ptr2)));
}
  
// Public
std::pair<int, bool> Lockfree_hash_table::search(int key) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  while (true) {
    Count_ptr ptr1 = table[0][h1];
    Hash_entry *e1 = ptr1->first;
    int ts1 = ptr1->second;

    if (e1 && e1->key == key)
      return std::make_pair(e1->val, true);

    Count_ptr ptr2 = table[1][h2];
    Hash_entry *e2 = ptr2->first;
    int ts2 = ptr2->second;

    if (e2 && e2->key == key)
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
  Count_ptr new_ptr = new std::pair<Hash_entry*, int>(new Hash_entry(), 0);
  Count_ptr ptr1, ptr2;

  int h1 = hash1(key);
  int h2 = hash2(key);

  while (true) {
    Find_result result = find(key, ptr1, ptr2);

    if (result == FIRST) {
      ptr1->first->val = val; 
      return;
    }

    if (result == SECOND) {
      ptr2->first->val = val;
      return;
    }

    if (!ptr1->first) { 
      new_ptr->second = ptr1->second;

      if (!__sync_bool_compare_and_swap(&table[0][h1], ptr1, new_ptr)) {
        continue; 
      }
      return;
    }

    if (!ptr2->first) { 
      new_ptr->second = ptr2->second;

      if (!__sync_bool_compare_and_swap(&table[1][h2], ptr2, new_ptr)) {
        continue; 
      }
      return;
    }

    result = relocate(0, h1);

  }
}

void Lockfree_hash_table::remove(int key) {
  int h1 = hash1(key);
  int h2 = hash2(key);

  Hash_entry ent1;
  Hash_entry ent2;

  Count_ptr ptr1 = new std::pair<Hash_entry*, int>(&ent1, 0);
  Count_ptr ptr2 = new std::pair<Hash_entry*, int>(&ent2, 0);
  Count_ptr swap_ptr = new std::pair<Hash_entry*, int>(nullptr, 0);

  while (true) {
    Find_result ret = find(key, ptr1, ptr2);

    if (ret == NIL) return;

    if (ret == FIRST) {
      swap_ptr->second = ptr1->second;
      if (__sync_bool_compare_and_swap(&table[0][h1], ptr1, swap_ptr)) {
        return;
      }
    } else if (ret == SECOND) {
      if (table[0][h1] != ptr1) 
        continue;
      swap_ptr->second = ptr2->second;
      if (__sync_bool_compare_and_swap(&table[1][h2], ptr2, swap_ptr))
        return;
    }
  }
}
