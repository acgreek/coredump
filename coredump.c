#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <wait.h>

static int g_pipe_fds[2];
static int g_child_pid;

static const char * gcore_path = "/usr/bin/gcore";

// this method is called as a result of signal so must only 
// containt async safe signal handler safe methods
static void request_core_dump(int sig) {
	if (5 != write(g_pipe_fds[1], "core", 5)) {
	}
	int status;
	if (g_child_pid != waitpid(g_child_pid, &status, 0)) {
	}
	close(g_pipe_fds[1]);
	_Exit(EXIT_FAILURE);
}

static void ReadCoreDumpSignalMessage(const char *path, int parent_pid) {
	signal(SIGPIPE, SIG_DFL);
	char pid_str[16];
	snprintf(pid_str, sizeof(pid_str)-1, "%d", parent_pid);
	char lpath[1024];
	strncpy(lpath,path, sizeof(lpath) -1);
	const char * array[] = {gcore_path, "-o", lpath, pid_str, NULL} ;
	char command[5];
  int offset = 0; 	
	while (offset != 5) {
		int status;
		offset += (status = read(g_pipe_fds[0], command + offset, 5 - offset));
		if (-1 == status) {
			exit(1);
		}
	}
	if (0 == strncmp("exit", command, 4)) {
		exit(0);
	}
	if (0 == strncmp("core",command, 4)) {
		fprintf(stderr, "parent process died\n");
		int rt = execvp(gcore_path, (char **) array);
		perror("");
		fprintf(stderr, "coredump process died %d\n", rt);
		exit(0);
	}
} 

int setup_core_dump_handler(const char *path) {
	int parent_pid = getpid();

	pipe(g_pipe_fds);
	if ((g_child_pid = fork()) == -1) {
		return -1;
	}
	if (g_child_pid == 0) {
		/* Parent process closes up input side of pipe */
		close(g_pipe_fds[1]);
		ReadCoreDumpSignalMessage(path, parent_pid);
	} else {
		/* Child process sloses up outputside of pipe */
#ifdef PR_SET_PTRACER
		prctl(PR_SET_PTRACER, g_child_pid, 0, 0, 0);
#endif
		close(g_pipe_fds[0]);
	}
	signal(SIGABRT, request_core_dump);
	signal(SIGQUIT, request_core_dump);
	signal(SIGSEGV, request_core_dump);
	signal(SIGFPE, request_core_dump);
	signal(SIGILL, request_core_dump);
	signal(SIGBUS, request_core_dump);
	signal(SIGSYS, request_core_dump);
	signal(SIGTRAP, request_core_dump);
	signal(SIGXCPU, request_core_dump);
	signal(SIGXFSZ, request_core_dump);
	return 0;
}

int fini_core_dump_handler(void){
	if (5 != write(g_pipe_fds[1], "exit",5)) {
	}
	close(g_pipe_fds[1]);
	int status;
	return g_child_pid == waitpid(g_child_pid, &status, 0);
}
