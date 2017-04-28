---
title: Checkpoint
layout: defaults
filename: checkpoint
---

## Checkpoint

### Summary

Project is going fairly well. Given our current timeline, we should be able to hit most of deliverables outlined in the proposal. The lock-free hash table implementation was the bulk of the work intended for this project as it includes many interesting ideas (path discovery, relocation marking, ABA counters, two-round querying, etc.). The optimizations we intend to implement on top are much more nitty-gritty. As of now, all planned optimizations are provisionally on the schedule, but we may decide to axe a few of them if they prove not to be of particular interest.

### Work Completed

We have implemented the following:
1. A fairly extensible testing framework for testing read/write throughput for both sequential and parallel hash tables.
2. A nearly complete implementation of the lock-free hash table based on Nguyen and Tsigas. Subject to additional work/testing.

### Updated Schedule

|---
| Date | Plan | Status
|-|-|-
|04/10-04/17 | Implement benchmarking/testing suite. Find interesting potential optimizations. Revise proposal as needed. | **Done**
|04/18-04/24 | Implement lock-free hash table. | **Done**
|04/25-04/28 | Finish lock-free hash table implementation. Test/benchmark results. | **In Progress**
|04/29-05/01 | Implement and test bucketized optimization. 
|05/02-05/05 | Implement and test optimizations based on Maier, et al. paper. 
|05/07-05/10 | Implement and test optimizations based on Shun paper. 
|05/11       | Finalize project report, prepare presentation. 

### Planned Demo

For the parallelism competition, we plan to show a graph with the results of our performance benchmarks and describe the improvements we made to the original lock-free hash table implementation.

### Concerns

1. Planned improvements to the initial lock-free hash table implementations may break invariants.
2. No preliminary results yet. It will be difficult to debug performance-related issues.
3. We haven't decided concretely on which optimizations to implement.

