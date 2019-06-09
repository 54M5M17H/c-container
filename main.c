#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sched.h>
#include <limits.h>
#include <sys/mount.h>
#include <sys/wait.h>

void run(char* executableArgs[]);
char** appendNull(char *args[], int argCount);
int childProc(void *args);
char** allocateArgs(char* args[], int argCount);

int main(int argCount, char* args[]) {
	if (argCount <= 1) {
		printf("No command \n");
		return 0;
	}

	char **executableArgs = allocateArgs(args, argCount);

	if (strcmp(args[1], "run") == 0) {
		run(executableArgs);
		return 0;
	}

	printf("Unknown command: %s \n", args[1]);
	return 0;
}

char** allocateArgs(char* args[], int argCount) {

	// pad one for NULL
	int execArgCount = argCount + 1;
	char **executableArgs = malloc(sizeof(args[0]) * execArgCount);

	// argCount - 2: because save last element for NULL
	for (int i = 0; i < execArgCount - 1; i++) {
		executableArgs[i] = args[i];
	}
	executableArgs[execArgCount - 1] = NULL;
	// skip first 2: 1) this file 2) run command
	return &executableArgs[2];
}

void run(char* executableArgs[]) {

	char *newRootPath = "/tmp/rootfs";
	int err = chroot(newRootPath);
	if (err == -1) {
		perror("Error chroot-ing");
		return;
	}

	err = chdir("/");
	if (err == -1) {
		perror("Error changing dir");
		return;
	}

	void *topOfChildStack = malloc(4096) + 4096;
	int namespaceFlags = CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS;
	pid_t pid = clone(childProc, topOfChildStack, SIGCHLD|namespaceFlags, executableArgs);
	if (pid == -1) {
		perror("Error cloning");
		return;
	}
	printf("New child process: %i \n", pid);

	pid = waitpid(pid, NULL, 0);
	printf("Process %i exited \n", pid);

	umount("proc");
	printf("Exiting... \n");
	return;
}

int childProc(void *args) {
	char **execArgs = args;
	printf("Hello from the child process... \n");
	int err = mount("proc", "proc", "proc", 0, "");
	if (err == -1) {
		perror("Error mounting");
		return err;
	}

	err = sethostname("inside_container", 16);
	if (err == -1) {
		perror("Error setting hostname \n");
		return err;
	}

	err = execv(execArgs[0], &execArgs[1]);
	if (err == -1) {
		perror("Exec error");
		return err;
	}
	return 0;
}
