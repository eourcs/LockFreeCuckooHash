#include "Lockfree_hash_table.h"

#define IS_MARKED(x) ((size_t)(x) % 2)

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
