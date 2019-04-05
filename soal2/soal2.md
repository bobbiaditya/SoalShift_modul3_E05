## Nomor 2
Pada soal ini, kami diminta untuk membuat sebuah client-server program untuk pembeli dan penjual. Pembeli dan penjual akan bisa mengakses variabel yang sama. Untuk itu, kami menggunakan shared memory.

Ada beberapa permintaan soal, seperti berikut:
- Terdapat 2 server: server penjual dan server pembeli
- 1 server hanya bisa terkoneksi dengan 1 client
- Server penjual dan server pembeli memiliki stok barang yang selalu sama
- Client yang terkoneksi ke server penjual hanya bisa menambah stok
- Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
- Server pembeli akan mengirimkan info ke client yang terhubung dengannya
apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
- Server penjual akan mencetak stok saat ini setiap 5 detik sekali
-  Menggunakan thread, socket, shared memory
  
### Server-penjual

####  Shared Memory
```c
void connect_sm(){
    key_t key = 1242;

    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    return;
}
```

Kodingan di atas berguna untuk membuat variable value supaya nanti bisa digunakan oleh semua program dengan key `1242`

#### Menerima pesan dari client penjual
```c
    while(1){
        valread = read(new_socket, buffer, 1024);

        char *status[2] = {"transaksi berhasil", "transaksi gagal"};
        // printf("[B] : %s, %d\n", buffer, valread);
        // to detect if socket had been closed
        if((strcmp(buffer, "tambah") == 0) && (valread != 0)){
            *value += 1;
            send(new_socket, status[0], strlen(status[0]), 0);
        } else {
            send(new_socket, status[1], strlen(status[0]), 0);
        }
    }
}
```
Dalam kodingan di atas, server akan menerima pesan yang dikirimkan oleh client. Jika pesan tersebut adalah string `tambah` maka, nilai value akan di tambah. Kemudian server akan mengirimkan pesan `transaksi berhasil`.

#### Thread
```c
void* multr(void *arg){
    pthread_t id = pthread_self();

    if(pthread_equal(id, tid[0])){
        time_t rawtime;
        struct tm * timeinfo;
        while(1){
            time(&rawtime);
            timeinfo = localtime(&rawtime);

            printf("%d : %s", *value, asctime(timeinfo));
            sleep(5);
        }
    } 
}
```
Thread ini berguna untuk menampilkan jumlah value saat ini beserta waktu saat ini.

### Server-Pembeli
####  Shared Memory
```c
void connect_sm(){
    key_t key = 1242;

    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    return;
}
```

Kodingan di atas berguna untuk membuat variable value supaya nanti bisa digunakan oleh semua program dengan key `1242`
#### Menerima pesan dari pembeli
```c
while(1){
    valread = read(new_socket, buffer, 1024);
    if((strcmp(buffer, "beli") == 0) && (valread != 0)){
        if(--new_value >= 0){
            *value = new_value;
            send(new_socket, status[0], strlen(status[0]), 0);
        } else {
            send(new_socket, status[1], strlen(status[0]), 0);
        }
        printf("%d\n", *value);
    } else {
        send(new_socket, status[1], strlen(status[0]), 0);
    }
}
```
Dalam kodingan di atas, server akan menerima pesan dari client pembeli. Jika pesan yang diterima adalah string `beli` maka value akan dikurangin 1, dan server akan mengirimkan pesan `transaksi berhasil`.


### Client
Client pembeli dan penjual memiliki code yang sama karena melakukan perintah-perintah yang sama.

#### Mengirimkan pesan
```c
while(1){
    strcpy(cmd, "");
    scanf("%s", cmd);
    send(sock, cmd, 10 , 0 );
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
}
```
Kodingan di atas berguna untuk mengirimkan pesan string kepada server.Kemudian menerima pesan dari server dalam bentuk string dan mencetaknya ke user.

### Hasil

Di bawah ini adalah hasil dari program server-penjual(kiri) dan client-penjual(kanan)

![Soal 2.1](/soal2/soal2_1.JPG)

Di bawah ini adalah hasil dari program server-pembeli(kiri) dan client-pembeli(kanan)

![Soal 2.2](/soal2/soal2_2.jpg)