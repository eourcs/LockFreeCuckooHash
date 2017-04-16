all:
	g++ -pthread src/benchmark/main.cpp -o benchmark -std=c++11
clean:
	rm benchmark
