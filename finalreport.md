---
title: Final Report
layout: defaults
filename: finalreport
---

## Final Report

### Summary

We implemented a fully-concurrent lock-free cuckoo hash table. On the Latedays cluster consisting of two six-core Xeon E5-2620 processors, our 24-thread implementation achieved ~13x speedup over a `C++11 std::unordered_map` and is competitive with Intel's `concurrent_hash_map` with little degradation in performance over various workloads.

Presentation slides summarizing our approach and findings can be found [here](https://cdn.rawgit.com/eourcs/LockFreeCuckooHash/471c5224/images/cuckoo-hash-slides.pdf).

### Background

#### Hash Tables

Hash tables are prevalent data structure used to implement key-value pair store and lookup. A general, high performance, concurrent hash table is an area of interest as many algorithms that would otherwise greatly benefit from parallelism are bottlenecked by hash table access. This is seen in the case of transposition tables in chess engines [1] and operation caches in the generation of binary decision diagrams [2].

#### Cuckoo Hashing

<p align="center">
  <img src="https://cdn.rawgit.com/eourcs/LockFreeCuckooHash/master/images/Cuckoo%20Path.svg" alt="Cuckoo Path"/>
</p>

Cuckoo hashing is a hashing scheme that uses two different hash functions to encode keys, providing two possible hash locations for each key. Lookup involves simply inspecting the two possible hash locations, so it is worst-case _O(1)_ (rather than expected). Insertions, if both hash locations are taken, involve a chain of relocations in which keys are pushed to their alternate hash locations to make space for the new key. With careful considerations to space utilization and hash functions, this procedure runs in expected _O(1)_ time. The relocation procedure is particularly difficult to make lock-free as any alterations to the "cuckoo path" should be observed and carefully considered to determine if a retry of the operation is required.

#### Lock-Freedom

Lock-free data structures often perform better under scenarios of high contention and when the machine architecture is unknown (and cannot be heavily optimized for). Since we are benchmarking against a general hash table implementation, lock freedom is an ideal approach for our hash table. Lock-free data structures, despite their performance characteristics, are notoriously difficult to implement as they necessitate a complex set of dependencies and invariants. In particular, the relocation operation, the ABA problem, and memory reclamation make guaranteeing correctness difficult.

### Approach

#### Data Structure

<p align="center">
  <img src="https://cdn.rawgit.com/eourcs/LockFreeCuckooHash/master/images/Data%20Structure.svg" alt="Data Structure"/>
</p>

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
The general data structure is fairly similar to that of other cuckoo hash tables. For the sake of simplicity, this only supports `(int, int)` key-value pairs, but parametrized variants are not difficult to implement. We allocate two tables which contain elements of the type `Count_ptr`, which, though typedef'ed, differs from `Hash_entry*`. By the x86-64 convention, the 16 most-significant-bits of an 8-byte pointer are not used in virtual addresses. In its place, we store a counter which keeps track of the number of relocations to address the ABA problem. This allows us to perform atomic memory writes with a single `__sync_bool_compare_and_swap` operation. Similarly, due to memory alignment requirements, the least-significant-bit in an address is always 0. In its place, we mark a bit to see if this entry is actively being relocated.

<p align="center">
  <img src="https://cdn.rawgit.com/eourcs/LockFreeCuckooHash/master/images/Pointer%20Structure.svg" alt="Pointer Structure"/>
</p>

To address memory reclamation, we also define the data structures necessary for the implementation of hazard pointers. They are, in essence, composed of a per-thread list of hash table entries that have been removed but not freed yet and and a per-thread array of potentially hazardous references.

#### Search

Though the search procedure in sequential implementations only requires lookup of two hash locations, we must be careful when dealing with concurrent environments. For example, consider a key `x` stored at `hash2(x)`. It is possible for this key to be relocated to `hash1(x)` after we have queried that location but before we query `hash2(x)`. A naive procedure would fail to find such a "moving key", despite it existing in the table.

To address this, we employ a two-round querying method. The first round queries the two hash locations and records the relocation counters. The second round performs the same operations and compares the relocation counters with that of the previous round. If the procedure cannot the find the key and we know that at most one relocation has occured between rounds, then we are guaranteed that the key does not exist in the table. Otherwise, it is possible that the key was missed when querying, so we retry the search procedure.

The implementations of remove and insert are conceptually similar. For the sake of brevity, we focus only on a special case of insert: relocate.

#### Relocate

When inserting a key, it is possible for both hash locations to be occupied. In this case, the original cuckoo hash implementation initiated a chain of relocations in which the new key is inserted into one of the tables, evicting a key. This "nestless key" is relocated to its alternate location, evicting another key. This procedure repeats until an empty slot is found (if such a slot cannot be found efficiently, the whole table is rehashed). However, this poses issues in concurrent environemnts. From the time it has been evicted to until it has evicted another key, the nestless key is absent from both tables and thus unreachable by other operations. To address this, we separate path discovery and path eviction. In the first step, we find an empty slot. In the second, we move the empty slot backwards along the cuckoo path by swapping with the previous key in the path. With this procedure, the "nestless key" is the empty slot being relocated, which of course does not impact correctness.

The "swap" operation is implemented as a series of `__sync_bool_compare_and_swap` operations. We first mark the relocation bit to alert other threads of the relocation procedure. Other threads operating on this entry cannot proceed until the relocation is complete, so we allow these threads to help in the relocation step. The entry is then copied to its destination slot and the source slot is cleared.

#### Hazard Pointers

In non garbage-collected lock-free environments, safe memory reclamation is not a trivial task. Due to the optimistic nature of lock-free data structures, different threads can access shared data under the assumption that this thread-local copy cannot be invalidated. However, consider the case in which a thread holds a `Hash_entry*` that has just been removed from the table. If the remove operation involves freeing the pointer, then it is possible for the thread to make an illegal memory access.

To address this, we implemented hazard pointers, a safe memory reclamation procedure for lock-free data structures. We allocate a global array which stores potentially hazardous references and a per-thread list of retired entries. Everytime a thread makes a potentially unsafe memory access, we add the pointer to the global array to ensure that other threads do not free the pointer. When removing entries, instead of free-ing immediately, we add the pointer to the `retired_list`. Once this list grows beyond a certain threshold, we iterate through the list and free entries that are not held by other threads (i.e. not in the global array). After careful consideration, it was observed that our implementation only makes two hazardous memory references at any given time. This means we can bound the size of the global array by a fixed constant, namely `thread_count * 2`.

### Results

We first benchmarked our implementation against a `C++11 unordered_map` and Intel's `concurrent_hash_map` on a stress test which completes 10,000,000 operations with randomly generated keys. The operations are randomly determined by a weighted probability distribution in which 90% of the operations are searches, 5% are inserts, and 5% are removes. Each table was initialized with a capacity of 10,000,000 elements and warmed-up to a load factor of 40%. This is because cuckoo hashing performance degrades quickly once the load factor exceeds 50%. While this is an interesting problem to address, this is a product of the algorithm, not of the lock-free implementation. All tests were performed on two six-core Xeon E5-2620 processors.

<p align="center">
<iframe width="640.3650657518581" height="396.008064516129" seamless frameborder="0" scrolling="no" src="https://docs.google.com/spreadsheets/d/1R00onU3EZGq5UJFM5aVaTk1c6dBQdpN6pHaS7GSN3kY/pubchart?oid=2120330578&amp;format=interactive"></iframe>
</p>

On this test bench, our implementation achieves 20,000 operations per millisecond, which is about 13x faster than a `C++11 unordered_map` and 1.25x faster than Intel's `concurrent_hash_map`. These are quite promising results, but we realized that randomly generated keys and operations may not be the most optimal test bench. We devised another test bench which, similarly, completes 10,000,000 operations with weighted probabilities, but guarantees that search and delete operations succeed (i.e. the element is in the table). We believe that this is more representative of a realistic workload. We note that raw throughput performance should not be used as an indicator as the benchmarks themselves have differing performance costs due to overhead. Instead, relative performance should be used as a metric.

<p align="center">
<iframe width="639.9612340690219" height="395.8036876098772" seamless frameborder="0" scrolling="no" src="https://docs.google.com/spreadsheets/d/1R00onU3EZGq5UJFM5aVaTk1c6dBQdpN6pHaS7GSN3kY/pubchart?oid=1309392171&amp;format=interactive"></iframe>
</p>

As we can see, we maintained similar speedup over a `C++11 unordered_map`, but now the figures for Intel's `concurrent_hash_map` and our implementation are much more similar. We believe that this is partly because many hash map implementations have different performance characteristics for successful and unsuccessful operations, while this is not the case for a cuckoo hash table. However, perhaps the more salient reason is that, based on some preliminary digging, this testbench generates fewer hash conflicts than the previous test bench. The performance characteristics for lock and lock-free data structures are similar under low contention scenarios, so this result aligns with our intuition. However, a large advantage of lock-free data structures is their relative performance under scenarios of high contention. In order to test this, we implemented a test bench which repeatedly reads from the same key.

<p align="center">
<iframe width="641.5" height="396.66083333333336" seamless frameborder="0" scrolling="no" src="https://docs.google.com/spreadsheets/d/1R00onU3EZGq5UJFM5aVaTk1c6dBQdpN6pHaS7GSN3kY/pubchart?oid=1484638631&amp;format=interactive"></iframe>
</p>

We observe that our lock-free cuckoo hash table greatly outperforms Intel's `concurrent_hash_map` under this test scenario. This is because Intel's implementation uses reader-writer locks which have much higher overhead than our two-round querying procedure. We also note that our implementation outperforms a `C++11 unordered_map`, though only by a factor of 3x. We believe that is largely because of the simplicity of cuckoo hashing, rather than any inherent advantage of the lock-free implementation.

### Future Work

#### Dynamic Resizing

Most implementations today implement resizing by locking the whole table and copying all of the data to a new table. In practice, this is relatively efficient since resizing occurs infrequently. However, this makes the worst-case cost of an operation tremendously high. Shun [3] proposes a scheme in which a new table is allocated, but only a constant fraction of the data is copied at a time. With careful choice of constants, the number of active tables can be bounded by two. This promises to be a much less disruptive resizing policy. Applying this same logic to the rehash operation may yield interesting results.

### Acknowledgments

Much of this work is derived from _Nguyen and Tsigas, 2014_. The `uint32_t -> uint32_t` hash functions were written by Bob Jenkins and Thomas Wang and taken from [here](https://gist.github.com/badboy/6267743). Documentation for Intel's `concurrent_hash_map` can be found [here](https://software.intel.com/en-us/node/506191).

Equal work was performed by both project members.

### Sources
* [1] ["A lockless transposition table implementation for parallel search"](http://www.craftychess.com/hyatt/hashing.html)
* [2] [Sylvan, "Multi-core Decision Diagrams"](http://www.tvandijk.nl/wp-content/uploads/2015/01/sylvan_tacas15.pdf)
* [3] [Shun, "Shared-Memory Parallelism Can Be Simple, Fast, and Scalable"](https://people.eecs.berkeley.edu/~jshun/thesis.pdf)
* [Li et al., "Algorithmic Improvements for Fast Concurrent Cuckoo Hashing"](https://www.cs.cmu.edu/~xia/resources/Documents/cuckoo-eurosys14.pdf)
* [Nguyen and Tsigas, "Lock-free Cuckoo Hashing"](http://excess-project.eu/publications/published/CuckooHashing_ICDCS.pdf)
* [Pagh, "Cuckoo Hashing"](http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=1B77D5F578DD4F628C1E982F717CD98E?doi=10.1.1.25.4189&rep=rep1&type=pdf)
* [Maier, Sanders, and Dementiev, "Concurrent Hash Tables: Fast and General(?)!"](https://arxiv.org/pdf/1601.04017.pdf)
