#ifndef COREDUMP_H
#define COREDUMP_H

void setup_core_dump_handler(const char * path);
void fini_core_dump_handler(void);

#endif  // COREDUMP_H
