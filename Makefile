all:
	g++ -pthread -Ilibcuckoo/install/include/ src/benchmark/main.cpp src/lockfree_hash_table.cpp -o benchmark -std=c++11
debug:
	g++ -pthread -Ilibcuckoo/install/include/ src/benchmark/main.cpp src/lockfree_hash_table.cpp -o benchmark -std=c++11 -g
clean:
	rm benchmark
