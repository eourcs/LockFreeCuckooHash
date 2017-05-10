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

Cuckoo hashing is a hashing scheme that uses two different hash functions to encode keys, providing two possible hash locations for each key. Lookup involves simply inspecting the two possible hash locations, so it is worst-case _O(1)_ (rather than expected). Insertions, if both hash locations are taken, involve a chain of relocations in which keys are pushed to their alternate hash locations to make space for the new key. With careful considerations to space utilization and hash functions, this procedure runs in expected _O(1)_ time. The relocation procedure is particularly difficult to make lock-free as any alterations to the "cuckoo path" should be observed and carefully considered to determine if a retry of the operation is required.

#### Lock-Freedom



### Approach

#### Data Structure

#### Search

#### Relocate

#### Hazard Pointers

### Results

### Future Work

### Sources
[1] http://www.craftychess.com/hyatt/hashing.html

[2] http://www.tvandijk.nl/wp-content/uploads/2015/01/sylvan_tacas15.pdf
