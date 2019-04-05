## Nomor 4

Pada soal nomor 4 kita diminta untuk:
- menyimpan 10 list proses ke dalam 2 file
- mengompres 2 file tersebut
- mengunzip 2 file tersebut

Dengan syarat pada saat menyimpan, kedua file disimpan secara bersamaan. Pada saat mengompres, kedua file dikompres secara bersamaan. Pada saat unzip, kedua file diunzip secara bersamaan

### Menyimpan list proses
```c
//fungsi membuat file
void bikin1()
{
	system("ps -aux|head -11 | tail -10 > $HOME/Documents/FolderProses1/SimpanProses1.txt");
}

void bikin2()
{
	system("ps -aux|head -11 | tail -10 > $HOME/Documents/
    FolderProses2/SimpanProses2.txt");	
}
//thread
void *lsproses(void *argv)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
                bikin1();
        }
        else if(pthread_equal(id,tid[1]))
        {
                bikin2();
        }
}
.
.
.
//pada main
//lsproses
while(i<2)
{
    err=pthread_create(&(tid[i]),NULL,&lsproses,NULL); //membuat thread
    if(err!=0) //cek error
    {
        //printf("\n can't create thread : [%s]",strerror(err));
    }
    else
    {
        //printf("\n create thread success\n");
    }
    i++;
}
pthread_join(tid[0],NULL);
pthread_join(tid[1],NULL);
```
Untuk menyimpan hasil ps -aux, kami menggunakan `system` dan kemudian menyimpan hasill ps- aux ke dalam folder yang ditentukan.

Agar kedua proses ps -aux bisa berjalan bersamaan, kami juga menggunakan join thread

### Zip File
```c
//fungsi zip file
void zip1()
{

	system("zip -rmj $HOME/Documents/FolderProses1/KompresProses1.zip $HOME/Documents/FolderProses1/SimpanProses1.txt");
	//printf("Menunggu 15 detik untuk mengekstrak kembali SimpanProses1\n");
}

void zip2()
{
	system("zip -rmj $HOME/Documents/FolderProses2/KompresProses2.zip $HOME/Documents/FolderProses2/SimpanProses2.txt");
	//printf("Menunggu 15 detik untuk mengekstrak kembali SimpanProses2\n");
}
//thread
void *zipfile(void *argv)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
                zip1();
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
                zip2();
        }
}
.
.
//Pada main
while(i<2)
{
    err=pthread_create(&(tid[i]),NULL,&zipfile,NULL); //membuat thread
    if(err!=0) //cek error
    {
        //printf("\n can't create thread : [%s]",strerror(err));
    }
    else
    {
        //printf("\n create thread success\n");
    }
    i++;
}	
pthread_join(tid[0],NULL);
pthread_join(tid[1],NULL);
```
Untuk melakukan zip file kami menggunakan `system` dan menggunakan fungsi `zip` dengan option `-rmj`. Option tersebut berguna agar pada saat melakukan zipping, file yang akan dizip akan langsung terhapus setelah proses zipping selesai. Selain itu juga berguna agar membuat hasil zip didalamnya terdiri dari 1 file `SimpanProses*` saja, tanpa ada direktori-direktori lainnya.

Agar kedua proses zip bisa berjalan bersamaan, kami juga menggunakan join thread.

### Unzip File
```c
///fungsi unzip
void unzip1()
{
	system("unzip $HOME/Documents/FolderProses1/KompresProses1.zip -d $HOME/Documents/FolderProses1/");
}

void unzip2()
{
	system("unzip $HOME/Documents/FolderProses2/KompresProses2.zip -d $HOME/Documents/FolderProses2/");
}

//Thread
void *unzipfile(void *argv)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
                unzip1();
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
                unzip2();
        }
}
.
.
.
//pada main
while(i<2)
{
    err=pthread_create(&(tid[i]),NULL,&unzipfile,NULL); //membuat thread
    if(err!=0) //cek error
    {
        //printf("\n can't create thread : [%s]",strerror(err));
    }
    else
    {
        //printf("\n create thread success\n");
    }
    i++;
}	
pthread_join(tid[0],NULL);
pthread_join(tid[1],NULL);
```
Untuk melakukan unzip kami menggunakan `system` dan fungsi `unzip` dengan option `-d`. Option tersebut digunakan agar hasil kompres bisa diarahkan ke folder yang diinginkan.

Agar kedua proses unzip bisa berjalan bersamaan, kami juga menggunakan join thread.


### Hasil
![Soal 4.1](/soal4/soal4_1.JPG)
![Soal 4.2](/soal4/soal4_2.JPG)



