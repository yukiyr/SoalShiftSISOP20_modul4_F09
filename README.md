LAPORAN RESMI
SISTEM OPERASI



KELOMPOK : F9

**Oleh:**

Yuki Yanuar Ratna

05111740000023

Rafif Ridho

05111840000058

**Asisten Pembimbing:**

Ibrahim Tamtama Adi

5111640000018

Departemen Teknik Infomatika

Fakultas Teknologi Elektro dan Informatika Cerdas

Institut Teknologi Sepuluh Nopember (ITS)

Surabaya

2020

**Soal**

1.	Enkripsi versi 1:

a. Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.

b. Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.

c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.

d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.

e. Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.

Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10
“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg
Note : Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt.

f. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.

**Jawaban :**

**Cara Pengerjaan**

**Encrypt**

```
char ext_cae[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_$

void encae(char *input){
        if(!strcmp(input,".") || !strcmp(input,"..")) return;

        for(i=0;i<strlen(input);i++)
        {
                for(ii=0;ii<strlen(ext_cae);ii++){
                        if(input[i]=='.')
                        {
                                return;
                        }
                        else if(input[i]==ext_cae[ii]){
                                input[i] = ext_cae[(ii+17)%strlen(ext_cae)];
                                break;
                        }
                }
        }
}
```

**Decrypt**

```
void decae(char *input){
        if(!strcmp(input,".") || !strcmp(input,"..")) return;

        for(int i=0;i<strlen(input);i++)
        {
                for(int ii=0;ii<strlen(ext_cae);ii++){
                        if(input[i]=='.')
                        {
                                return;
                        }
                        else if(input[i]==ext_cae[ii]){
                                input[i] = ext_cae[(ii+strlen(ext_cae)-17)%strlen(ext_cae)];
                                break;
                        }
                }
        }
}
```

**Kendala Yang Dialami**

-

**Screenshot**

**Soal**

2.	Enkripsi versi 2:

a. Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.

b. Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.

c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi direktori tersebut akan terdekrip.

d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.

e. Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada direktori asli akan menjadi 5 file kecil yakni: File_Contoh.txt.000, File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan File_Contoh.txt.004.

f. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada didalam direktori tersebut (rekursif).

**Jawaban :**

**Cara Pengerjaan**

**Kendala Yang Dialami**

-

**Screenshot**

**Soal**

3.	Sinkronisasi direktori otomatis:

Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:

a. Kedua directory memiliki parent directory yang sama.

b. Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi:

   - Nama dari setiap berkas di dalamnya sama.

   - Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.

c. Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja.

d. Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory.

Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi.

Implementasi dilarang menggunakan symbolic links dan thread.


**Jawaban :**

**Cara Pengerjaan**

**Kendala Yang Dialami**

**Screenshot**

**Soal**

4.	Log system:

a. Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.

b. Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.

c. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.

d. Sisanya, akan dicatat dengan level INFO.

e. Format untuk logging yaitu:

```
[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]
```


LEVEL    : Level logging

yy        : Tahun dua digit

mm         : Bulan dua digit

dd         : Hari dua digit

HH         : Jam dua digit

MM         : Menit dua digit

SS         : Detik dua digit

CMD          : System call yang terpanggil

DESC      : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::)


Contoh format logging nantinya seperti:

```
INFO::200419-18:29:28::MKDIR::/iz1
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg
```

**Jawaban :**

**Cara Pengerjaan**

**Level Warning**

```
void logWarning(char *input)
{
    FILE* fp;
    char data1[50];
    fp = fopen("/home/test/fs.log", "a");

    time_t raw;
    struct tm *timeinfo;
    char tanggal[40];
    time(&raw);
    timeinfo = localtime(&raw);

    strftime(tanggal, sizeof(tanggal), "%y%m%d-%H:%M:%S", timeinfo); 
    sprintf(data1,"WARNING::%s::%s\n", tanggal, input);
    fputs(data1, fp);

    fclose(fp);
}
```

**Level Info**

```
void logInfo(char *input)
{
    FILE* fp;
    char data1[50];
    fp = fopen("/home/test/fs.log", "a");

    time_t raw;
    struct tm *timeinfo;
    char tanggal[40];
    time(&raw);
    timeinfo = localtime(&raw);

    strftime(tanggal, sizeof(tanggal), "%y%m%d-%H:%M:%S", timeinfo); 
    sprintf(data1,"INFO::%s::%s\n", tanggal, input);
    fputs(data1, fp);

    fclose(fp);
}
```

**Kendala Yang Dialami**

**Screenshot**
