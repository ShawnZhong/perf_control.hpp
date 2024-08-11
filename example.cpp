#include "perf_control.hpp"

void foo() { for (volatile int i = 0; i < 100000000; i++); }
void bar() { for (volatile int i = 0; i < 100000000; i++); }

int main() {
  PerfControl perf({"perf", "record"});
  foo();
  perf.enable();
  bar();
  perf.disable();
  foo();
}