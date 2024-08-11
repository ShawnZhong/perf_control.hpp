# perf_control.hpp
Control perf to enable or disable recording

# Usage

```cpp
PerfControl pc({"perf", "record"});
pc.enable();
// do something
pc.disable();
```

# Example

See [example.cpp](example.cpp) for an example:

```sh
g++ example.cpp -o example
./example
perf report
```

Output:

``` 
# Overhead  Command  Shared Object     Symbol                                        
# ........  .......  ................  ..............................................
#
    96.48%  example  example           [.] bar
```