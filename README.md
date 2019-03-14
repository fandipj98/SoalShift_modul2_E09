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
Pertama – tama kita membuat proses daemon, kemudian pada menit awal (menit 0), kita membuat folder dengan format /home/fandipj/log/[dd:MM:yyyy-hh:mm] dengan [dd:MM:yyyy-hh:mm] adalah waktu sekarang. Kemudian setiap 30 menit, kita membuat folder dengan format yang [dd:MM:yyyy-hh:mm] yang baru dan yang sesuai dengan waktu sekarang dan setiap 1 menit, kita mecopy file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log dengan # dimulai dari 1.
