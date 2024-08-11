# perf_control.hpp
Control perf recording dynamically during program execution. 

This header-only library provides a simple interface to enable or disable perf event recording at specific points in the code, allowing for targeted performance analysis.

# Usage

```cpp
PerfControl pc({"perf", "record"});
pc.enable();
// do something
pc.disable();
```

# Example

Consider the following example:

https://github.com/ShawnZhong/perf_control.hpp/blob/eb99420160c4b1daa72c6b175206b21fa43ed66c/example.cpp#L7-L12

```sh
$ g++ example.cpp -o example

$ ./example
Events disabled
Events enabled
Events disabled
[ perf record: Woken up 4 times to write data ]
[ perf record: Captured and wrote 0.057 MB perf.data (939 samples) ]

$ perf report
# Overhead  Command  Shared Object     Symbol                                        
# ........  .......  ................  ..............................................
#
    96.48%  example  example           [.] bar
```

The function `foo()` is not recorded by perf, while `bar()` is captured.