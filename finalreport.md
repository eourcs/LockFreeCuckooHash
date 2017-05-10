---
title: Final Report
layout: defaults
filename: finalreport
---

## Final Report

### Summary

We implemented a fully-concurrent cuckoo hash table. On the Latedays cluster consisting of two six-core Xeon E5-2620 processors, our 24-thread implementation achieved ~13x speedup over a `C++11 std::unordered_map` with little degradation in performance over various workloads.

### Background

#### Hash Tables

Hash tables are prevalent data structure used to implement key-value pair store and lookup. A robust, high performance, concurrent hash table is an area of interest as many algorithms that would otherwise greatly benefit from parallelism are bottlenecked by hash table access. This is seen in the case of transposition tables in chess engines [1] and operation caches in the generation of binary decision diagrams [2].

#### Cuckoo Hashing

Cuckoo hashing is a hashing scheme that uses two different hash functions to encode keys, providing two possible hash locations for each key. Lookup involves simply inspecting the two possible hash locations, so it is worst-case O(1) (rather than expected). Insertions, if both hash locations are taken, involve a chain of relocations in which keys are pushed to their alternate hash locations to make space for the new key. With careful considerations to space utilization and hash functions, this procedure runs in expected O(1) time. The relocation procedure is particularly difficult to make lock-free as any alterations to the "cuckoo path" should be observed and carefully considered to determine if a retry of the operation is required.

#### Lock-Freedom

Lock-free data structures often perform better under scenarios of high contention and when the machine architecture is unknown (and cannot be heavily optimized for). Since we are benchmarking against a general hash table implementation, lock freedom is an ideal approach for our hash table. Lock-free data structures, despite their performance characteristics, are notoriously difficult to implement as they necessitate a complex set of dependencies and invariants. In particular, the relocation operation, the ABA problem, and memory reclamation make guaranteeing correctness difficult.

### Approach

#### Data Structure

```
// Cuckoo hash table data structure
Count_ptr table[2];

struct Hash_entry {
  int key;
  int val;
};

// Use unused bits for relocation marking/counting
typedef Hash_entry* Count_ptr;

// Hazard pointers data structure
std::vector<std::vector<Hash_entry*>>   retired_list;
std::vector<int>                        retired_count;

std::vector<std::array<Hash_entry*, 2>> hazard_pointer_record;  
```
The general data structure is fairly similar to that of other cuckoo hash tables. We allocate two tables which contain elements of the type `Count_ptr`, which, though typedef'ed, differs from `Hash_entry*`. By the x86-64 convention, the most significant 16 bits of an 8-byte pointer are not used in virtual addresses. In its place, we store a counter which keeps track of the number of relocations to address the ABA problem. This allows us to perform atomic memory writes with a single `__sync_bool_compare_and_swap` operation. Similarly, due to memory alignment requirements, the lowest significant bit in an address is always 0. In its place, we mark a bit to see if this entry is actively being relocated.

To address memory reclamation, we also define the data structures necessary for the implementation of hazard pointers. They, in essence, are composed of a per-thread list of hash table entries that have been removed but not freed yet and and a per-thread array of potentially hazardous references.

#### Search



#### Relocate

#### Hazard Pointers

### Results

### Future Work

### Sources
[1] http://www.craftychess.com/hyatt/hashing.html

[2] http://www.tvandijk.nl/wp-content/uploads/2015/01/sylvan_tacas15.pdf
