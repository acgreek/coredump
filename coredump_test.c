#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "coredump.h"

int main(int argc,char * argv[]) {
	setup_core_dump_handler("dump");
	sleep(3);
	kill(getpid(), SIGSEGV);	
	fini_core_dump_handler();
	return 0;
}
