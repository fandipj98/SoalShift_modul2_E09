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
int status,tanggal,jam,menit,detik,tanggal1,jam1,menit1,detik1,selisih,flag=0;
time_t t; 
struct tm *tmp;
char s[100],date[100],now[100],path[100],pathtemp[100],source[100]="/home/fandipj/Documents/makanan"; 

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
  counter=0;

  while(1) {
    // main program here
    
    time( &t );
    tmp = localtime( &t ); 
    memset(now,0,sizeof(now));
    memset(date,0,sizeof(date));
    memset(s,0,sizeof(s));
    memset(date,0,sizeof(date));

    strftime(now, sizeof(now), "%d%H%M%S", tmp);  
    
    strcpy(path, source);
    strcpy(pathtemp, source);
    strcat(pathtemp, "/makan_enak.txt");

    struct stat *file_info = malloc(sizeof(struct stat));
    if (lstat(pathtemp, file_info) != 0) {

    }

    strftime(date, sizeof(date), "%d%H%M%S", localtime(&file_info->st_atime));
    tanggal=(date[0]-'0')*10;
    tanggal+=date[1]-'0';    
    jam=(date[2]-'0')*10;
    jam+=date[3]-'0';
    menit=(date[4]-'0')*10;
    menit+=date[5]-'0';
    detik=(date[6]-'0')*10;
    detik+=date[7]-'0';

    tanggal1=(now[0]-'0')*10;
    tanggal1+=now[1]-'0';    
    jam1=(now[2]-'0')*10;
    jam1+=now[3]-'0';
    menit1=(now[4]-'0')*10;
    menit1+=now[5]-'0';
    detik1=(now[6]-'0')*10;
    detik1+=now[7]-'0';

    if(tanggal1-tanggal<=1){
      if(tanggal1-tanggal==1){
        jam1=24;
      }
      selisih=(jam1-jam)*3600+(menit1-menit)*60+detik1-detik;
      if(selisih<=30){
        flag=1;
      }
      else{
        flag=0;
      }
    }
    sleep(5);
    if (flag == 1) {
      counter++;
      sprintf(s,"makan_sehat%d.txt",counter);
      strcat(path,"/");
      strcat(path, s);
      FILE *pfile;
      pfile = fopen(path,"a");
      fclose(pfile);
    }
  }
  exit(EXIT_SUCCESS);
}
