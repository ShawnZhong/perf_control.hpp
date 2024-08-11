.PHONY: all run report clean
all: example report


example: example.cpp perf_control.hpp
	g++ example.cpp -o example

run: example
	./example

report: run
	perf report | cat

clean:
	rm -f example perf.data