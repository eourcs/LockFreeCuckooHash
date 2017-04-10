#include <utility>

struct Hash_table {
  std::pair<int, bool> search(int key); 
  void                 insert(int key, int val);
  void                 remove(int key);
};
