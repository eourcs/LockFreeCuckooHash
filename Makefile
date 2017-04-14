all:
	g++ src/main.cpp src/std_hash_table.cpp src/lockfree_hash_table.cpp -o benchmark -std=c++11
clean:
	rm benchmark
