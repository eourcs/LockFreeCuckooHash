all:
	g++ src/main.cpp -o benchmark -std=c++11
clean:
	rm benchmark
