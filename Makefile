.PHONY: all run report clean
all: prep run report

prep:
	sudo sysctl kernel.perf_event_paranoid=-1

example: example.cpp perf_control.hpp
	g++ example.cpp -o example

run: example
	./example

report: run
	perf report | cat

clean:
	rm -f example perf.data