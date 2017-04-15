#include "hash_table.h"
#include "std_hash_table.h"

Std_hash_table::Std_hash_table(int capacity) {
  map = std::unordered_map<int, int>(capacity);
}

std::pair<int, bool> Std_hash_table::search(int key) {
  try {
    return std::make_pair(map.at(key), true);
  } catch (std::exception e) {
    return std::make_pair(0, false);
  }
}

void Std_hash_table::insert(int key, int val) {
  map[key] = val; 
}

void Std_hash_table::remove(int key) {
  map.erase(key);
}
