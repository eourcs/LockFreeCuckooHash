#include "Lockfree_hash_table.h"
#include <cstdint>

// Inline bit twiddling functions
inline uint16_t get_counter(Count_ptr ptr) { 
  return (uint16_t)(((size_t)ptr >> 48) & 0xFFFF);
}

inline Count_ptr set_counter(Count_ptr ptr, uint16_t counter) {
  return (Count_ptr)((size_t)ptr | (size_t)counter);
}

inline bool is_null_entry(Count_ptr ptr) {
  return ((size_t)ptr & 0xFFFFFFFFFFFF) != 0;
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

void rehash() {

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
    Count_ptr e1 = table[0][h1];
    int ts1 = get_counter(e1);

    if (!is_null_entry(e1)) {
      if (get_marked(e1)) {
        help_relocate(0, h1, false);
        continue; 
      }

      if (e1->key == key) 
        result = FIRST; 
    }

    Count_ptr e2 = table[1][h2];
    int ts2 = get_counter(e2);

    if (!is_null_entry(e2)) {
      if (get_marked(e2)) {
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

    if (check_counter(ts1, ts2, get_counter(ptr1), get_counter(ptr2))) {
      continue;
    } else {
      return NIL;
    }
  }
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
    Count_ptr e1 = table[0][h1];
    int ts1 = get_counter(e1);

    if (!is_null_entry(e1) && e1->key == key)
      return std::make_pair(e1->val, true);

    Count_ptr e2 = table[1][h2];
    int ts2 = get_counter(e2);

    if (!is_null_entry(e2) && e2->key == key)
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
      e1->val = val; 
      return;
    }

    if (result == SECOND) {
      e2->val = val;
      return;
    }

    if (is_null_entry(e1)) { 
      if (!__sync_bool_compare_and_swap(
            &table[0][h1], e1, set_counter(new_node, get_counter(e1)))) {
        continue; 
      }
      return;
    }

    if (is_null_entry(e2)) { 
      if (!__sync_bool_compare_and_swap(
            &table[1][h2], e2, set_counter(new_node, get_counter(e2)))) {
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
            &table[0][h1], e1, set_counter(NULL, get_counter(e1)))) {
        return;
      }
    } else if (ret == SECOND) {
      if (table[0][h1] != e1) 
        continue;
      if (__sync_bool_compare_and_swap(
            &table[1][h2], e2, set_counter(NULL, get_counter(e2)))) {
        return;
      }
    }
  }
}
