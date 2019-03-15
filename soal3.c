#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main() {
	int status;
	pid_t child_id;
	child_id = fork();
	if (child_id == 0) {
		execlp("/usr/bin/unzip", "unzip", "campur2.zip", NULL);
	}
	while (wait(&status) > 0);

	int fileout = open("daftar.txt", O_RDWR | O_CREAT, 0777);

	int pipes[2];
	pipe(pipes);

	child_id = fork();
	if (child_id == 0) {
		dup2(pipes[1], 1);
		for (int i = 0; i < 2; i++)
			close(pipes[i]);
		execlp("ls", "ls", "campur2", NULL);
	}
	child_id = fork();
	if (child_id == 0) {
		dup2(pipes[0], 0);
		dup2(fileout, 1);

		for (int i = 0; i < 2; i++)
			close(pipes[i]);

		execlp("grep", "grep", ".txt$", NULL);
	}
	for (int i = 0; i < 2; i++)
		close(pipes[i]);
	while (wait(&status) > 0);
	while (wait(&status) > 0);
	return 0;
}