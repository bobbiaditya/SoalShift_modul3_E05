## Nomor 1

Pada soal nomor 1 kita diminta untuk melakukan perhitungan fibonacci dari inputan bilangan-bilangan yang ditentukan, lalu menampilkan hasil perhitungannya secara berurutan

### Hitung Faktorial
```c
int fakto(int n)
{
	int hasil=1;
	int i;

	if(n==0) return 0;
	for(int i=1;i<=n;i++)
	{
		hasil=hasil*i;
	}

	return hasil;
}
```
Kodingan di atas akan mereturn nilai hasil dari proses perhitungan fibonaci

### Proses Sorting
```c
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void bubbleSort(int arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
}
```
Kodingan di atas berguna untuk melakukan sorting. Algoritma sorting yang kami gunakan adalah bubble sort. Sorting ini kemudian akan digunakan untuk sorting hasil dari proses perhitungan fibonacci tiap-tiap angka

### Thread
```c
void *jalanin(void *arg)
{
    pthread_t id=pthread_self();
	FOR(i,jumlah)
	{
	    if(pthread_equal(id, tid[i])) 
	    {
	    	hasil[i]=fakto(temp[i]);
	    }
	}
}
.
.
.
   FOR(i, jumlah)
    {
		pthread_join(tid[i],NULL);
    }
```
Untuk setiap input angka akan membuat sebuah thread baru. Masing-masing thread akan menjalankan fungsi perhitungan faktorial dan menyimpannya ke array hasil. Selain itu juga saya melakukan join thread untuk semua thread yang telah dibuat agar thread bisa jalan bersama


### Scan angka
```c
int main(int argc, char *argv[])
{
	int i=0;
	int err;
	jumlah=argc;
	for(i=0;i<jumlah;i++)
	{
		temp[i]=atoi(argv[i]);
		//printf("%d\t",temp[i]);
	}
.
.
.
```
Pada kodingan di atas saya mengubah input angka yang awalnya saya simpan dalam bentuk char menjadi int dengan menggunakan fungsi `atoi()`

### Print Hasil
```c
for(int i=1;i<jumlah;i++)
{
    printf("%d!=%d\n",temp[i],hasil[i]);
}
```

setelah melakukan perhitungan faktorial dan proses sorting, maka hasil akan diprint

### Hasil
![Soal 1](/soal1/soal1.JPG)


