# Laporan Resmi Sistem Operasi Modul2 E09

## Nomor 4
### Soal:

4. Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik). Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.
<br/>Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt
<br/>Catatan: 
dilarang menggunakan crontab
Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Jawaban:
Pertama - tama kita membuat proses daemon, kemudian mengambil waktu akses terakhir kali dari file makan_enak.txt yang terdapat dalam direktori /home/fandipj/Documents/makanan dengan menggunakan `struct stat` dan `strftime()` seperti syntax dibawah ini:
```
  char pathtemp[100],source[100]="/home/fandipj/Documents/makanan";
  strcpy(path, source);
  strcpy(pathtemp, source);
  strcat(pathtemp, "/makan_enak.txt");

  struct stat *file_info = malloc(sizeof(struct stat));
  if (lstat(pathtemp, file_info) != 0) {

  }

  strftime(date, sizeof(date), "%d%H%M%S", localtime(&file_info->st_atime));
```
Kemudian kita kita juga mengambil waktu sekarang dengan menggunakan `struct tm` dan `strftime()` seperti syntax dibawah ini:
```
  time_t t; 
  struct tm *tmp;
  time( &t );
  tmp = localtime( &t ); 
  strftime(now, sizeof(now), "%d%H%M%S", tmp);
```
Kemudian kita mengconvert waktu akses terakhir kali dari file makan_enak.txt dan waktu sekarang menjadi integer dengan menggunakan syntax seperti berikut ini:
```
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
```
Kemudian kita menghitung selisih dari waktu sekarang dengan waktu akses terakhir kali dari file makan_enak.txt berdasarkan satuan detik, jika selisihnya kurang dari atau sama dengan 30 detik, maka kita membuat file makan_sehat#.txt di dalam direktori /home/fandipj/Documents/makanan setiap 5 detik dengan # dimulai dari 1 dan diincrement sebanyak 1 per 5 detik dengan syntax seperti berikut ini:
```
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
```
## Nomor 5
### Soal:

5. Kerjakan poin a dan b di bawah:
a) Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
<br/>Ket:
<br/>- Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
<br/>- Per menit memasukkan log#.log ke dalam folder tersebut
<br/>‘#’ : increment per menit. Mulai dari 1
<br/>b)Buatlah program c untuk menghentikan program di atas.
<br/>NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

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
