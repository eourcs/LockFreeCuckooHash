#!/bin/sh
./benchmark -o temp.txt
python3 graph.py temp.txt
rm temp.txt
