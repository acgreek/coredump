#include <sys/types.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <wait.h>
static int pipe_fd[2];
static int childpid;

void segv_handler(int sig) {
	write(pipe_fd[1], "core",5);
	int status;
	waitpid(childpid, &status, 0);
}

void setup_core_dump_handler(const char *path) {
	int parent_pid = getpid();

	pipe(pipe_fd);

	if((childpid = fork()) == -1) {
		exit(1);
	}

	if(childpid == 0) {
		char pid_str[16];
		sprintf(pid_str,"%d", parent_pid);
		char lpath[1024];
		strcpy(lpath,path);
		char * array[] = {"/usr/bin/gcore", "-o", lpath, pid_str, NULL} ;
		/* Parent process closes up input side of pipe */
		close(pipe_fd[1]);
		char command[5];
		read(pipe_fd[0], command,5);
		if (0 == strncmp("exit",command, 4) ) {
			exit(0);
		}
		if (0 == strncmp("core",command, 4) ) {
			printf("parent process died\n");
			int rt = execvp("/usr/bin/gcore", array);
			perror("");
			printf("coredump process died %d\n", rt);
			exit(0);
		}

	} else {
		/* Child process sloses up outputside of pipe */
		prctl(PR_SET_PTRACER, childpid, 0, 0, 0);
		close(pipe_fd[0]);
	}
	signal(SIGSEGV,  segv_handler);
}
