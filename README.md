# coredump

This little library provides an api to replace the default segv handler with one that will generate a core regardless of the system 
wide Linux kernel/ulimit settings. 


void setup_core_dump_handler(const char * path);

sets up the core dump handler. Path is the full path to where the core dump will written if a segv signal is received. 


void fini_core_dump_handler(void);

you should call this before program termination 


Example 


#include "coredump.h"

int main(int argc, char * argv[]) {
  setup_core_dump_handler("/tmp/example_core");
  
  .... program body here ....
  
  fini_core_dump_handler();
  return 0;
}
