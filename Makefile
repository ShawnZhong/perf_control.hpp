.PHONY: all run report clean
all: run report

example: example.cpp perf_control.hpp
	g++ example.cpp -o example

run: example
	./example

report: run
	perf report -i perf.data | cat

clean:
	rm -f example perf.data