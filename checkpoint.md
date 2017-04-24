---
title: Checkpoint
---

### Work Completed
We have implemented the following:
1. A fairly-extensible testing framework for testing read/write throughput for both sequential and parallel hash tables.
2. A nearly-complete implementation of the lock-free hash table based on Nguyen and Tsigas. Subject to additional work/testing.

### Updated Schedule
|---
| Date | Plan | Status
|-|-|-
|04/10-04/17 | Implement benchmarking/testing suite. Find interesting potential optimizations. Revise proposal as needed. | Done.
|04/18-04/24 | Implement lock-free hash table. | Partially complete.
|04/25-04/28 | Finish lock-free hash table implementation. Test/benchmark results.
|04/29-05/01 | Implement and test bucketized optimization.
|05/02-05/05 | Implement and test optimizations based on Maier, et al. paper.
|05/07-05/10 | Implement and test optimizations based on Shun paper.
|05/11       | Finalize project report, prepare presentation.

### Planned Demo

For the parallelism competition, we plan to show a graph with the results of our performance benchmarks and describe the improvements we made to the original lock-free hash table implementation.

### Concerns

* Planned improvements to the initial lock-free hash table implementations may break invariants
* We haven't decided on which improvements we will make to the initial implementation.
