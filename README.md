### Summary

We are going to implement a fully concurrent lock-free hash table implementation based on _Nguyen and Tsigas_. We hope to improve upon their design with optimizations suggested in the literature. This implementation will be benchmarked against other hash table implementations under various read/write workloads.

### Background

Hash tables are an incredibly common data structure with a wide variety of use cases, including removing duplicates, computing edge contraction, and generating binary decision diagrams. A common variant of the hash table uses "cuckoo hashing," which employs two distinct hash functions and guarantees worst case _O(1)_ lookup. 

The search for a highly-performant general purpose hash table is a very active and robust field of study. Recent implementations by _names_ employ either employ coarse and fine grained locking, or are lock-free but only under a constrained set of operations. In 2014, _Nguyen and Tsigas_ published what they believed to be the world's first implementation of a fully-concurrent lock-free hash table. Under their experiments, their design proved to be faster than the current state of the art.

We wish to improve upon their implementation by combining various optimizations suggested in the literature.

### Resources
We will be writing our implementation from scratch in C++ based on a paper entitled 
[Lock-free Cuckoo Hashing](http://excess-project.eu/publications/published/CuckooHashing_ICDCS.pdf) (Nguyen, Tsigas) and will be extending this implementation
with several optimizations, some taken from the thesis entitled 
[Shared-Memory Parallelism Can Be Simple, Fast, and Scalable](https://people.eecs.berkeley.edu/~jshun/thesis.pdf) (Shun 2015). 
We will run benchmarks for our implementations on a latedays node and should not 
require any additional resources.

### Goals and Deliverables
//TODO

### Platform Choice
The latedays cluster is appropriate for benchmarking our implementations because it 
provides a high degree of potential
parallelism, with 24 execution contexts on each node, while also being easily accessible to us.
It will allow us to test the performance of our implementations with a variety
of thread counts, providing more complete and interesting results than what could
be gathered on other available CMU machines. 

C++ is an appropriate language for our implementation because
it is very capable of producing high-performance code, and we are both familiar with
it after using it for previous assignments in 15418.

### Schedule
Date | Plan | Status
---- | ---- | ------
04/10-04/17 | Implement benchmarking/testing suite. Find interesting potential optimizations. Revise proposal as needed.  | 
04/18-04/24 | Implement lock-free hash table | 
04/25-05/01 | One week reserved for debugging, testing, etc. |
05/02-05/09 | Implement and test optimizations | 

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/eourcs/LockFreeCuckooHash/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
