# Laporan Resmi Sistem Operasi Modul2 E09

## Nomor 5
### Soal:

5. Kerjakan poin a dan b di bawah:
a) Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
<br/>Ket:
<br/>- Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
<br/>- Per menit memasukkan log#.log ke dalam folder tersebut
<br/>‘#’ : increment per menit. Mulai dari 1
<br/>b)Buatlah program c untuk menghentikan program di atas.

### Jawaban:
Pertama – tama kita membuat proses daemon, kemudian pada menit awal (menit 0), kita membuat folder dengan format /home/fandipj/log/[dd:MM:yyyy-hh:mm] dengan [dd:MM:yyyy-hh:mm] adalah waktu sekarang. Syntax untuk mendapatkan waktu sekarang adalah seperti berikut ini:
```
  time_t t; 
  struct tm *tmp;
  time( &t );
  tmp = localtime( &t ); 
  strftime(date, sizeof(date), "%d:%m:%Y-%H:%M", tmp);
      
```
Syntax untuk membuat folder adalah dengan menggunakan mkdir() dari C seperti berikut ini:
```
  char path[100],source[100]="/home/fandipj/log/"; 
  strcpy(path, source);
  strftime(date, sizeof(date), "%d:%m:%Y-%H:%M", tmp);
  strcat(path,date);            
  mkdir(path, 0777);
```
Kemudian setiap 30 menit, kita membuat folder dengan format yang [dd:MM:yyyy-hh:mm] yang baru dan yang sesuai dengan waktu sekarang dan setiap 1 menit, kita mengcopy file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log dengan # dimulai dari 1 dan diincrement sebanyak 1 per menitnya. Untuk mengcopy file log tersebut, maka digunakan `fork()` untuk membuat child process setiap 1 menit dan pada setiap child processnya dijalankan `execv("/bin/cp", argv)` seperti syntax dibawah ini:
```
  child_id = fork();
  sleep(60);
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
```
Kemudian untuk menghentikan program diatas maka digunakan syntax seperti berikut ini:
```
  char *argv[4] = {"pkill", "-e", "soal5", NULL};
  execv("/usr/bin/pkill", argv);
```
