all:
	g++ -pthread -Itbb-2017_U5/include -Ltbb-2017_U5/build/macos_intel64_clang_cc8.0.0_os10.11.6_release src/benchmark/main.cpp src/lockfree_hash_table.cpp -ltbb -o benchmark -std=c++11
debug:
	g++ -pthread -Itbb-2017_U5/include -Ltbb-2017_U5/build/macos_intel64_clang_cc8.0.0_os10.11.6_release src/benchmark/main.cpp src/lockfree_hash_table.cpp -ltbb -o benchmark -std=c++11 -g
clean:
	rm benchmark
