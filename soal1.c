#include <sys/types.h>
#include <sys/stat.h>
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
    int status;
    DIR* dir = opendir("/home/fms/modul2/gambar");
    if (dir) {
      closedir(dir);
    }
    else {
      mkdir("/home/fms/modul2", 0777);
      mkdir("/home/fms/modul2/gambar", 0777);
    }

    char *ptrToSubString;
    char fileName[100];
    char fileOut[100];
    char *dirs = "/home/fms/modul2/gambar/";
    char grey[] = "_grey.png";
    struct dirent *ent;
    dir = opendir(".");
    while((ent = readdir(dir)) != NULL) {
     memset(fileName, 0, sizeof fileName);
     memset(fileOut, 0, sizeof fileOut);
     strcpy(fileName,ent->d_name);
     ptrToSubString = strstr(fileName,".png");
     if (ptrToSubString == NULL)
       continue;
     if (strcmp(ptrToSubString, ".png") == 0) {
      printf("%s\n", fileName);
      char tmp[100];
      strncpy(tmp, fileName, strlen(fileName) - 4);
      strcat(fileOut, dirs);
      strcat(fileOut, tmp);
      strcat(fileOut, grey);
      rename(fileName, fileOut);
     } else {
      continue;
     }
    }
    closedir(dir);
    sleep(10);
  }
  
  exit(EXIT_SUCCESS);
}
