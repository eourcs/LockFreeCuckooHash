---
title: Proposal
---

## Proposal

### Summary

We are going to implement a fully concurrent lock-free hash table implementation based on Nguyen and Tsigas [3]. We hope to improve upon their design with optimizations suggested in the literature. This implementation will be benchmarked against other hash table implementations under various read/write workloads.

### Background

Hash tables are an incredibly common data structure with a wide variety of use cases, including removing duplicates, computing edge contraction, and generating binary decision diagrams. A common variant of the hash table uses "cuckoo hashing," which employs two distinct hash functions and guarantees worst case _O(1)_ lookup. 

The search for a highly-performant general purpose hash table is a very active and robust field of study. Recent implementations either employ coarse and fine grained locking [1], or are lock-free but only under a constrained set of operations [2]. In 2014, _Nguyen and Tsigas_ published what they believed to be the world's first implementation of a fully-concurrent lock-free hash table. Under their experiments, their design proved to be faster than the current state of the art.

We wish to improve upon their implementation by combining various optimizations suggested in the literature.

### Challenges
Our problem is challenging because lock-free programming requires a complex set of 
dependencies and invariants, so extending the existing implementations will require
great care. Also, our extensions will almost certainly be original contributions to
the field, so we will not have much prior work to draw from.

### Resources
We will be writing our implementation from scratch in C++ based on a paper by Nguyen and Tsigas [3] and will be extending this implementation
with several optimizations, some taken from Julian Shun's thesis [2]. 
We will run benchmarks for our implementations on a latedays node and should not 
require any additional resources.

### Goals and Deliverables
We plan to achieve an implementation of a concurrent lock-free hash table with better
performance on one of our chosen metrics (throughput, space utilization, possibly others) compared to the existing lock-free implementation as well as other fast hash table implementations. We hope to achieve an implementation with better performance on all metrics. 
We will evaluate wall clock performance against std::unordered_map and the 
baseline lock-free cuckoo hash table, using a variety of insert/update/remove
access patterns and a variety of thread counts. Our demo will consist of graphs demonstrating the results of our benchmarks. 

### Platform Choice
The latedays cluster is appropriate for benchmarking our implementations because it 
provides a high degree of potential
parallelism, with 24 execution contexts on each node, while also being easily accessible to us.
It will allow us to test the performance of our implementations with a variety
of thread counts, providing more complete and interesting results than what could
be gathered on other available CMU machines. 

C++ is an appropriate language for our implementation because
it is very capable of producing high-performance code, and we are both familiar with
it after using it for previous assignments in 15-418.

### Schedule

|---
| Date | Plan | Status
|-|-|-
|04/10-04/17 | Implement benchmarking/testing suite. Find interesting potential optimizations. Revise proposal as needed.
|04/18-04/24 | Implement lock-free hash table. 
|04/25-05/01 | One week reserved for debugging, testing, etc.
|05/02-05/09 | Implement and test optimizations.
|05/11       | Finalize project report, prepare presentation.

### References
* [1] [Li et al., "Algorithmic Improvements for Fast Concurrent Cuckoo Hashing"](https://www.cs.cmu.edu/~xia/resources/Documents/cuckoo-eurosys14.pdf)
* [2] [Shun, "Shared-Memory Parallelism Can Be Simple, Fast, and Scalable"](https://people.eecs.berkeley.edu/~jshun/thesis.pdf)
* [3] [Nguyen and Tsigas, "Lock-free Cuckoo Hashing"](http://excess-project.eu/publications/published/CuckooHashing_ICDCS.pdf)
