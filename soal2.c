#include <sys/types.h>
#include <sys/stat.h>
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
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/fms/Documents")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    char* filename = "/home/fms/Documents/hatiku/elen.ku";
    struct stat info;
    int status = stat(filename, &info);
    if (status == 0) {
      struct passwd *pw = getpwuid(info.st_uid);
      struct group *gr = getgrgid(info.st_gid);
      // printf("%s %s\n", pw->pw_name, gr->gr_name);
      // printf("%d %d\n", strcmppw->pw_name, gr->gr_name);
      if (!strcmp(pw->pw_name, "www-data") && !strcmp(gr->gr_name, "www-data")) {
        chmod(filename, 0777);
        remove(filename);
      }
    }
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
}