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



#### Lock-Freedom

### Our Approach

#### Data Structure

#### Search

#### Relocate

#### Hazard Pointers

### Results

### Future Work

### Sources
