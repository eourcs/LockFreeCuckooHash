all:
	g++ -pthread src/benchmark/main.cpp src/lockfree_hash_table.cpp -o benchmark -std=c++11
clean:
	rm benchmark
