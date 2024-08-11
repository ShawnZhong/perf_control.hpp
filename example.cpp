#include "perf_control.hpp"

void foo() { for (volatile int i = 0; i < 100000000; i++); }
void bar() { for (volatile int i = 0; i < 100000000; i++); }

int main() {
  PerfControl perf({"perf", "record", "-o", "perf.data"});
  foo();
  perf.enable();
  bar();  // Only record this
  perf.disable();
  foo();
}