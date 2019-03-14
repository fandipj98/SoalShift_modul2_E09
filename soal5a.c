#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

pid_t child_id;
int status;
time_t t; 
struct tm *tmp;
struct stat sb;
char s[100],date[100],path[100],source[100]="/home/fandipj/log/"; 
int counter;


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

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  counter = 0;

  while(1) {
    // main program here
    time( &t );
    tmp = localtime( &t ); 
    if(counter==0){
      strcpy(path, source);
      if(stat(path, &sb)==0 && S_ISDIR(sb.st_mode)){
      
      }
      else{
        mkdir(path, 0777);
      }
      strftime(date, sizeof(date), "%d:%m:%Y-%H:%M", tmp);
      strcat(path,date);
      mkdir(path, 0777);
      counter++;
    }

    child_id = fork();
    
    sleep(2);
    if (child_id == 0 && counter!=0) {
      // this is child
      sprintf(s,"log%d.log",counter);
      strcat(path,"/");
      strcat(path, s);
      char *argv[4] = {"cp", "/var/log/syslog", path, NULL};
      execv("/bin/cp", argv);
    }else {
      // this is parent

      // the parent waits for all the child processes
      while ((wait(&status)) > 0);
      counter++;
      if(counter==31){
        counter = 0;
      }
    }
  }
  exit(EXIT_SUCCESS);
}
