## Soal 5

Pada soal ini, kami menggunakan multithread dan shared memory. Selain itu, kami menggunakan *library* `ncurses.h` untuk tampilannya.

Secara umum, kita akan membagi beberapa program untuk melakukan input, menampilkan status, menampilkan menu, dan juga memperbarui variabelnya.

### Struktur Data

Data monster disimpan dalam struct yang memiliki spesifikasi.

```c
struct monster {
    char name[50];
    int status[3], status_flag[3];

    int foodstock;
};
```

Variabel `name` merupakan nama monster, `status` berupa nilai status dari masing-masing elemen yaitu secara terurut hunger, hygiene, dan health. `status_flag` digunakan untuk menentukan apakah status akan melakukan regenerasi setiap waktu. `foodstock` merupakan stok dari makanan yang kita miliki.

Struct monster digunakan untuk monster pemain dan NPC.

### Thread

Pada dasarnya kita akan memakai enam thread, yaitu ...

```c
pthread_create(&(tid[0]), NULL, &maintain_interface, NULL);
pthread_create(&(tid[1]), NULL, &maintain_hunger, NULL);
pthread_create(&(tid[2]), NULL, &maintain_hygiene, NULL);
pthread_create(&(tid[3]), NULL, &maintain_health, NULL);
pthread_create(&(tid[4]), NULL, &maintain_bath, NULL);
pthread_create(&(tid[5]), NULL, &maintain_life, NULL);
```

Jadi nantinya setiap thread akan dibind di fungsi masing-masing untuk memaintain sesuatu. Seperti contohnya, `maintain_interface` digunakan untuk maintain tampilan antarmuka, `maintain_health` untuk maintain darah dari monster.

### Interface

Tampilan antarmuka ditangani oleh thread `maintain_interface`. Tampilan secara umum, akan dibuatkan dalam sebuah window, yang akan menampilkan judul, status, dan menu.

Secara umum tampilan nantinya akan terbagi menjadi beberapa scene. Scene tersebut antara lain:
1. Scene Standby
2. Scene Battle
3. Scene Shop (Buyer)

Selain itu, ada scene lain yaitu Scene Shop (Merchant) yang dipisah di program lain.

Dari masing-masing scene, nantinya akan dibagi lagi menjadi `(scene)-status` untuk menampilkan status dari masing-masing scene, dan `(scene)-menu` untuk menampilkan pilihan menu yang ada dan metode inputnya.

#### Tampilan Antarmuka 

Untuk tampilan antarmuka sendiri kami membuat *helper function* untuk memudahkan pembuatan tampilan di masing-masing scene.

```c
void scene_init(char * title){
    werase(win);

    box(win, 0, 0);
    mvwprintw(win,  0, 2, "[ %s SCENE ]", title);
    mvwprintw(win, 23, 2, "[ %s ]", mpp.name);
}
````

#### Menampilkan Status

Untuk menampilkan status, kita mencetak nilai dari masing-masing status.

```c
int scene_idle_status(){
    switch_status_flag(1);

    clear_status();

    mvwprintw(win, 2, 4, "HUNGER     : %03d", mpp.status[0]);
    mvwprintw(win, 3, 4, "HYGIENE    : %03d", mpp.status[1]);
    mvwprintw(win, 4, 4, "HEALTH     : %03d", mpp.status[2]);
    mvwprintw(win, 5, 4, "FOOD LEFT  : %03d", mpp.foodstock);
    mvwprintw(win, 6, 4, "BATH CD    : %03d", bath_c[1]);

    wrefresh(win);
}
```

Modularisasi fungsi dari status idle ditujukan agar nantinya ketika setiap thread yang melakukan perubahan di variabel status, tinggal memanggil fungsi status untuk memperbarui data status di tampilan, tanpa perlu memanggil fungsi scene.

Untuk dapat memanggil masing-masing fungsi untuk refresh kita menyimpan fungsi status sekarang ke variabel `scene_current_status`, sehinga nantinya kita hanya perlu memanggil `scene_current_status()`.

#### Tampilan Menu

Untuk tampilan menu, selain menampilkan menu, kita juga menunggu *input* dari pemain. Untuk memudahkan membuat menu dari masing-masing scene, kami menggunakan *helper function* `menu()`.

```c
void menu(char ** choices, int (**routes) (), int (**statuses) (), int ch_c){
    int base = 10;

    FOR(i, ch_c){
        mvwprintw(win, base+i, 4, "%s", choices[i]);
    }

    mvwprintw(win, 20, 4, " > ");    

    wrefresh(win);

    // Input
    char c;
    while((c = getch())){
        if(c >= '1' && c <= ch_c+'1') break;
    }

    // Route handling
    FOR(i, ch_c){
        if(i == (int)c - (int)('1')){
            clear_status();
            pthread_mutex_lock(&mutex_sc);
            scene_current_status = statuses[i];
            pthread_mutex_unlock(&mutex_sc);
            ((routes[i])) ();
        }
    }
}
```

Dari kode di atas, kita memerlukan parameter `choices`, `routes`, `status`, dan `ch_c`. Choices adalah deskripsi dari masing-masing menu. Routes adalah fungsi yang akan dipanggil dari masing-masing menu. Status menyimpan fungsi status dari masing-masing choices yang nantinya digunakan untuk memperbarui `scene_current_status`.

#### Standby Scene

Pada scene kali ini, kita harus melakukan fungsi regenerasi untuk variabel hunger, hygiene, dan health. Regenerasi dilakukan di masing-masing thread `maintain_(var)`.

```c
void *maintain_hygiene(void *arg){
    while(1){
        sleep(30);

        if(mpp.status_flag[1] != 0){
            mpp.status[1] -= 10;
            scene_current_status ();
        }
    }
}
```
Dalam contoh fungsi untuk maintain hygiene di atas, pertama kita sleep sesuai waktu yang ditentukan, kemudian kita mengubah nilai dari status hygiene yang berada pada `mpp.status[1]`. Fungsi untuk maintain hunger dan health sendiri cukup mirip dengan fungsi `maintain_hygiene` di atas.

Selain memaintain status dari monster sendiri, kita juga perlu memaintain status dari cooldown mandi. 

```c
int scene_idle_menu(){
    char *menus[] = {"1. GIVE FOOD",
                     "2. TAKE A BATH",
                     "3. BATTLE SCENE", 
                     "4. SHOP SCENE (BUYER)", 
                     "5. EXIT"};

    int (*routes[5]) () = {eat, bath, 
                           scene_battle, scene_shop_buyer, 
                           route_exit};

    int (*statuses[5]) () = {scene_idle_status, scene_idle_status,
                             scene_battle_status, scene_shop_buyer_status, 
                             scene_idle_status};

    menu(menus, routes, statuses, 5);
}
```

Untuk pilihan menu di scene sendiri ada beberapa pilihan, dengan pilihan masing-masing akan dipetakan ke suatu fungsi di `routes`.

Untuk fungsi `eat()` dan `bath()` sendiri nantinya ketika fungsi telah selesai dieksekusi akan kembali ke `scene_idle` lagi.

```c
int eat(){
    if(mpp.foodstock != 0){
        pthread_mutex_lock(&mutex);
        mpp.status[0] += 15;
        mpp.status[0] = (mpp.status[0] > 200) ? 200 : mpp.status[0];
        pthread_mutex_unlock(&mutex);
        mpp.foodstock--;
    }

    scene_idle();
}

int bath(){
    pthread_mutex_lock(&mutex_b);
    if(bath_c[0] != 0){ // Bisa dipakai
        mpp.status[1] += 30;
        bath_c[0] = 0;
        bath_c[1] = 20;
    }

    pthread_mutex_unlock(&mutex_b);

    scene_idle();
}
```
#### Battle Scene

Secara umum, kode untuk scene battle sendiri seperti scene sebelumnya, hanya saja pilihan yang disediakan hanya dua yaitu **Attack** atau **Run**. Pilihan Run akan mengakibatkan pemain kembali ke menu standby. Sedangkan untuk pilihan Attack, nantinya akan menjalankan fungsi di bawah, yang kemudian akan kembali lagi ke Battle Scene.

```c
int battle_attack(){
    mpp.status[2] -= 20;
    npc.status[2] -= 20;

    if(mpp.status[2] <= 0 || npc.status[2] <= 0){
        scene_idle();
    }

    scene_current_status();
    scene_battle_menu();

    getch();
}
```

#### Shop (Buyer) Scene

Kode secara garis besar, seperti kode Battle Scene sebelumnya, hanya saja ada dua kemungkinan **Buy** atau **Back**. Pilihan Back akan mengembalikan kita ke standby scene. Sedangkan untuk pilihan Buy, akan melakukan kode di bawah.

```c
int shop_buy(){
    if(*shop_foodstock != 0){
        pthread_mutex_lock(&mutex_fsf);
        *shop_foodstock -= 1;
        pthread_mutex_unlock(&mutex_fsf);
        mpp.foodstock++;
    }

    scene_shop_buyer();
}
```

Pada kode di atas, kita akan membeli makanan dari toko dengan stok yang sudah ada. Variabel `shop_foodstock` merupakan nilai yang dibagi antara dua proses, berupa game ini dan program merchant. Pembagian dari variabel tersebut menggunakan *shared memory*.

#### Die Scene

Ada kalanya monster yang kita miliki tidak memiliki status yang baik dan mengakibatkan pada kematiannya, oleh karena itu ada Die Scene. Untuk scene ini akan dimaintain oleh thread `maintain_life`.

```c
void *maintain_life(void *arg){
    while(1){
        sleep(1);

        FOR(i, 3){
            if(mpp.status[i] <= 0){
                pthread_mutex_lock(&mutex_sc);  
                scene_current_status = scene_die_status;
                pthread_mutex_unlock(&mutex_sc);

                scene_die();
            }
        }
    }
}
```

Pada potongan kode di atas, thread akan secara terus menerus mengecek apakah ada salah satu komponen status yang telah mencapai nol. Jika ditemui ada status yang memiliki nilai nol, maka akan dialihkan ke die scene.

```c
int scene_die_status(){
    clear_status();

    mvwprintw(win, 10, 4, "YOUR MONSTER DIE!");
    mvwprintw(win, 12, 4, "Press x button to exit...");

    wrefresh(win);
}

int scene_die(){
    scene_init("MONSTER DIE");

    scene_die_status();

    while(getch() != 'x');

    endwin();
    curs_set(1);
    exit(0);
}
```
### Merchant Shop Program

Pada program *merchant* ini, kita akan mengubah memanipulasi variabel `shop_foodstock` yang dibagi dengan game.

```c
int *shop_foodstock = 0;
key_t key = 4242;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
shop_foodstock = shmat(shmid, NULL, 0);
```

Untuk pilihan yang mungkin adalah perintah restock dan keluar. Untuk perintah restock sendiri akan mengeksekusi kode di bawah.

```c
int shop_restock(){
    *shop_foodstock += 1;

    scene_shop_merchant();
}
```

Pada program di atas, hanya melakukan manipulasi shared memory, sedangkan untuk menginisialisasi shared memory sendiri, kami membuat fungsi bantuan lainnya (`soal5i.c`).

### Tampilan Layar

![soal5/soal5a.png](soal5/soal5a.png)
![soal5/soal5b.png](soal5/soal5b.png)
![soal5/soal5c.png](soal5/soal5c.png)
![soal5/soal5d.png](soal5/soal5d.png)
![soal5/soal5e.png](soal5/soal5e.png)
![soal5/soal5f.png](soal5/soal5f.png)