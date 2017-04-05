### Summary

We are going to implement a fully concurrent lock-free hash table implementation based on _Nguyen and Tsigas_. We hope to improve upon their design with optimizations suggested in the literature. This implementation will be benchmarked against other hash table implementations under various read/write workloads.

### Background

Hash tables are an incredibly common data structure with a wide variety of use cases, including removing duplicates, computing edge contraction, and generating binary decision diagrams. A common variant of the hash table uses "cuckoo hashing," which employs two distinct hash functions and guarantees worst case _O(1)_ lookup. 

The search for a highly-performant general purpose hash table is a very active and robust field of study. Recent implementations by _names_ employ either employ coarse and fine grained locking, or are lock-free but only under a constrained set of operations. In 2014, _Nguyen and Tsigas_ published what they believed to be the world's first implementation of a fully-concurrent lock-free hash table. Under their experiments, their design proved to be faster than the current state of the art.

We wish to improve upon their implementation by combining various optimizations suggested in the literature.

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
