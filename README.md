# Laporan Resmi Sistem Operasi Modul2 E09

## Nomor 1
### Soal:

1. Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi "`[namafile]_grey.png`". Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori `/home/[user]/modul2/gambar`. Catatan : Tidak boleh menggunakan crontab.

### Jawaban:
Awalnya kita buat proses daemon terlebih dahulu. Kemudian pada main program, kita memeriksa apakah direktori tujuan telah terbuat atau belum dengan menggunakan perintah `opendir` pada C. Jika sudah dibuat, kita dapat melanjutkannya. Jika tidak, 
kita buat direktori tersebut dengan perintah `mkdir`. Sehingga syntaxnya seperti ini:
```
DIR* dir = opendir("/home/fms/modul2/gambar");
if (dir) {
  closedir(dir);
}
else {
  mkdir("/home/fms/modul2", 0777);
  mkdir("/home/fms/modul2/gambar", 0777);
}

```
Lalu, langkah selanjutnya adalah memeriksa setiap file pada current directory. Jika ekstensi file tersebut berupa `.png`, maka kita perlu menambah `_grey` pada nama file tersebut lalu memindahkannya ke `/home/[user]/modul2/gambar`. Untuk iterasi setiap file pada directory tersebut, kita dapat menggunakan perintah `readdir([directory])`. Untuk memeriksa apakah file tersebut memiliki ekstensi `.png` dapat dilakukan dengan mencari substring `.png` dengan perintah `strstr`. Perlu diperhatikan bahwa perintah `strstr` akan mengembalikan sebuah string baru dengan pada kemunculan pertama string yang dicari hingga akhir. Sehingga kita perlu memeriksa apakah string akhir tersebut hanya berupa `.png` atau tidak. Untuk hal tersebut cukup menggunakan perintah `strcmp` yang bernilai sama dengan 0 saat kedua string yang di komparasi bernilai sama. Langkah selanjutnya adalah menyisipkan `_grey` pada filename. Hal tersebut dapat dilakukan dengan mengiris filename awal sebelum ekstensi, menambahkan `_grey` pada akhir string tersebut, dan menambahkan kembali ekstensinya. Sehingga secara keseluruhan syntaxnya menjadi seperti ini:
```
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
``` 
Langkah terakhir adalah menambahkan waktu untuk pengulangan background proses. Karena tidak dicantumkan di soal, cukup menambahkan angka aman seperti 10 / 15 detik.
```
sleep(10);
```
## Nomor 2
### Soal:

2. Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on. Catatan: Tidak boleh menggunakan crontab.

## Jawaban:
Pertama-tama kita membuat daemonnya terlebih dahulu. Setelah itu kita memeriksa apakah file tersebut ada atau tidak dengan menggunakan perintah `stat`. Perintah tersebut akan mengembalikan nilai 0 jika ada dan -1 jika tidak. Setelah itu, jika file tersebut ada, maka kita harus mencari user dan group pemilik file tersebut. Untuk user, kita dapat menggunakan perintah `getpwuid([uid dari file]` dan mengakses `pw_name` dari hasil tersebut. Untuk group dapat menggunakan perintah `getgrgid[gid dari file]` dan mengakses `gr_name` dari hasil. Setelah itu, kita memeriksa apakah user dan group pemilik file tersebut adalah `www-data`. Untuk itu, dapat digunakan perintah `strcmp` seperti halnya pada jawaban nomor 1. Jika kondisi diatas benar, maka untuk menghapusnya kita perlu mengubah permissionnya terlebih dahulu menggunakan perintah `chmod[mode yang diinginkan (dalam kasus ini 0777)]`. Setelah itu cukup melakukan perintah `remove`. Sehingga syntaxnya menjadi seperti berikut:
```
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

```
Karena program tersebut harus dapat berjalan setiap 3 detik sekali, maka cukup tambahkan perintah 
```
sleep(3);
```
## Nomor 3
### Soal:

3. Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”.
Buatlah program C yang dapat : 
  i)  mengekstrak file zip tersebut. 
  ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.  
  Catatan:  
    ○ Gunakan fork dan exec. 
    ○ Gunakan minimal 3 proses yang diakhiri dengan exec. 
    ○ Gunakan pipe 
    ○ Pastikan file daftar.txt dapat diakses dari text editor 

## Jawaban:

Langkah pertama adalah meng-unzip file campur2.zip. Kita dapat melakukan `fork()` lalu lakukan perintah unzip dengan menggunakan perintah `execlp` pada C dengan parameter `"unzip", "unzip", "campur2.zip", NULL` dan wait hingga child prosesnya selesai. Syntaxnya sebagai berikut:
```
pid_t child_id;
child_id = fork();
if (child_id == 0) {
  execlp("/usr/bin/unzip", "unzip", "campur2.zip", NULL);
}
while (wait(&status) > 0);
```
Setelah itu, kita buat redirection untuk `daftar.txt` dengan perintah `open`, dengan parameter `O_RWRD (Untuk Read dan Write) | O_CREAT (Untuk create jika belum ada), 0777 (Permission)`. Perlu diperhatikan bahwa permission 0777 agar file dapat diakses dari text editor. Syntax tersebut jg sekaligus menyimpan variabel file yang nantinya akan di redirect. Syntaxnya seperti ini:
```
int fileout = open("daftar.txt", O_RDWR | O_CREAT, 0777);
```
Langkah selanjutnya adalah membuat 2 pipe, pipe pertama sebagai input bagi `grep`, dan pipe kedua sebagai output bagi `ls`.
syntaxnya:
```
int pipes[2];
pipe(pipes);
```
Sampai saat ini, perlu diingat yang akan kita jalankan adalah `ls | grep .txt$ > daftar.txt`

Proses selanjutnya adalah membuat proses `ls` nya. Cukup melakukan `fork()` lalu memasukkan hasil `execlp("ls", "ls", "campur2", NULL)` ke pipe tersebut. Perintah `dup2` berguna untuk menduplikasi pipe sebelumnya, dengan parameter kedua memiliki nilai streamnya. argumen 0 adalah stdin, argumen 1 adalah stdout, argumen 3 adalah stderr. Syntaxnya seperti ini:
```
child_id = fork();
if (child_id == 0) {
  dup2(pipes[1], 1);
  for (int i = 0; i < 2; i++)
    close(pipes[i]);
  execlp("ls", "ls", "campur2", NULL);
}
  ```
Proses selanjutnya adalah membuat proses `grep` dari pipe sebelumnya. Cukup melakukan `fork()` lalu memasukkan hasil pipe tadi dengan perintah `dup`, menyiapkan pipe untuk keluar dengan perintah `dup` dan mengeksekusi perintah `grep` dengan perintah `execlp`. Perlu diperhatikan bahwa perintah grep yang akan dijalankan untuk memeriksa file yang berekstensi `.txt` adalah `grep .txt$` dollar sign sebagai pembatas akhir dalam suatu regular expression. Sehingga parameternya menjadi `execlp("grep", "grep", ".txt$", NULL)`. Syntaxnya menjadi:
```
child_id = fork();
if (child_id == 0) {
  dup2(pipes[0], 0);
  dup2(fileout, 1);

  for (int i = 0; i < 2; i++)
    close(pipes[i]);

  execlp("grep", "grep", ".txt$", NULL);
}
```
Jangan lupa untuk menutup pipe pada parent process dengan menambahkan syntax:
```
for (int i = 0; i < 2; i++)
  close(pipes[i]);
```
Perintah terakhir adalah menunggu child proses berhenti, karena kita menjalankan 2 child proses(`ls` dan `grep`) maka kita memerlukan 2 wait()
```
while (wait(&status) > 0);
while (wait(&status) > 0);
```
Kesimpulan
```
Proses yang dilakukan ada 3 : (`unzip`, `ls`, dan `grep`) Sekaligus menggunakan `fork()` dan `exec()`.
Menggunakan `pipe` untuk komunkasi antara proses `ls` dan `grep`.
Permission file `daftar.txt` diubah menjadi 0777 agar dapat diakses siapapun.
```
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
