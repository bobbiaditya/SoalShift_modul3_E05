#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define FOR(i, k) for(int i=0; i<k; i++)
#define FORR(i, j, k) for(int i=j; i<=k; i++)
#define LEN(a) sizeof(a)/sizeof(a[0])
#define HEIGHT 24
#define WIDTH 64

key_t key = 4242;

static int counter = 0;
const int pos_start[2] = {0, 1};
pthread_t tid[6] = {0};
WINDOW * win;
WINDOW * status;
int (*scene_current_status)();

int *shop_foodstock = 0;
int bath_c[2] = {1, 0};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_sc = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_fsf = PTHREAD_MUTEX_INITIALIZER;

struct monster {
    char name[50];
    int status[3], status_flag[3];

    // int bath_c[2]; // [0] availabilty flag, [1] cooldown
    int foodstock;
};

struct monster mpp, npc;

// UTILITIES

void m_init(struct monster * x, int a, int b, int c);
void clear_status();
void menu(char ** choices, int (**routes) (), int (**statuses) (), int ch_c);
void scene_init(char * title);
int route_exit();
void switch_status_flag(int x);

// THREAD FUNCTIONS

void *maintain_interface(void *arg);
void *maintain_life(void *arg);
void *maintain_bath(void *arg);
void *maintain_hunger(void *arg);
void *maintain_hygiene(void *arg);
void *maintain_health(void *arg);

// SCENES

int eat();
int bath();

int battle_attack();
int battle_run();

int shop_buy();
int shop_restock();

int scene_idle_status();
int scene_battle_status();
int scene_shop_buyer_status();
int scene_shop_merchant_status();

int scene_idle_menu();
int scene_battle_menu();
int scene_shop_buyer_status();
int scene_shop_merchant_status();
int scene_die_status();

int scene_givename();

int scene_idle();
int scene_battle();
int scene_shop_buyer();
int scene_shop_merchant();
int scene_die();

int init_window();

int main()
{   
    // SHM
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shop_foodstock = shmat(shmid, NULL, 0);  

    // CURSES
    m_init(&mpp, 200, 100, 300);

    init_window();
    scene_givename();

    noecho();
    curs_set(0);

    scene_current_status = scene_idle_status;

    // THREAD

    pthread_create(&(tid[0]), NULL, &maintain_interface, NULL);
    pthread_create(&(tid[1]), NULL, &maintain_hunger, NULL);
    pthread_create(&(tid[2]), NULL, &maintain_hygiene, NULL);
    pthread_create(&(tid[3]), NULL, &maintain_health, NULL);
    pthread_create(&(tid[4]), NULL, &maintain_bath, NULL);
    pthread_create(&(tid[5]), NULL, &maintain_life, NULL);

    FOR(i, 6) pthread_join(tid[i], NULL);

    return 0;
}

// UTILITY

int init_window(){
    initscr();

    win = newwin(HEIGHT, WIDTH, pos_start[0], pos_start[1]);    
}

void m_init(struct monster * x, int a, int b, int c){
    int tmp_status[3] = {a, b, c};
    FOR(i, 3){
        x->status[i] = tmp_status[i];
        x->status_flag[i] = 1;
    }

    x->foodstock = 0;
}

void clear_status(){
    FOR(i, 6){
        mvwprintw(win, 2+i, 4, "                                            ");
    }
}

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

void scene_init(char * title){
    werase(win);

    // Window
    box(win, 0, 0);
    mvwprintw(win,  0, 2, "[ %s SCENE ]", title);
    mvwprintw(win, 23, 2, "[ %s ]", mpp.name);
}

int scene_givename(){    
    char name[50];

    move(1, 2);
    scanw("%s", name);

    strcpy(mpp.name, name);

    refresh();
}

int route_exit(){
    erase();
    endwin();
    curs_set(1);

    exit(0);

    return(0);
}

void switch_status_flag(int x){
    FOR(i, 3){
        // mpp.status[i] = mpp.status[i] ^ 1;
        mpp.status_flag[i] = x;
    }
}

// -

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

void *maintain_interface(void *arg){
    scene_idle();

    route_exit();
}

void *maintain_bath(void *arg){
    while(1){
        if(bath_c[0] == 0){ // Tidak bisa dipakai
            // endwin();
            // exit(0);
            while(bath_c[1] != 0){
                sleep(1);
                pthread_mutex_lock(&mutex_b);
                bath_c[1]-=1;
                pthread_mutex_unlock(&mutex_b);
                scene_current_status();
            }
            pthread_mutex_lock(&mutex_b);
            bath_c[0] = 1;
            pthread_mutex_unlock(&mutex_b);
        }
    }
}

void *maintain_life(void *arg){
    while(1){
        sleep(1);

        // Karena di soal tidak tertulis
        // kalau health = 0, pemain akan kalah
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

void *maintain_hunger(void *arg){
    while(1){
        sleep(10);

        if(mpp.status_flag[0] != 0){
            pthread_mutex_lock(&mutex);
            mpp.status[0] -= 5;
            pthread_mutex_unlock(&mutex);
            scene_current_status ();
        }
        refresh();
    }
}

void *maintain_hygiene(void *arg){
    while(1){
        sleep(30);

        if(mpp.status_flag[1] != 0){
            mpp.status[1] -= 10;
            scene_current_status ();
        }
    }
}

void *maintain_health(void *arg){
    while(1){
        sleep(10);

        if(mpp.status_flag[2] != 0){
            mpp.status[2] += 5;
            scene_current_status ();
        }
    }
}

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

int scene_idle(){
    scene_init("STANDBY");

    // Status
    scene_idle_status();
    scene_idle_menu();

    return 0;
}

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

int scene_battle_status(){
    switch_status_flag(0);

    mvwprintw(win, 2, 4, "YOUR  HEALTH : %03d", mpp.status[2]);
    mvwprintw(win, 3, 4, "ENEMY HEALTH : %03d", npc.status[2]);

    wrefresh(win);
}

int scene_battle_menu(){
    char *menus[] = {"1. ATTACK", 
                     "2. RUN"};

    int (*routes[]) () = {battle_attack, scene_idle};

    int (*statuses[]) () = {scene_battle_status, scene_idle_status};

    menu(menus, routes, statuses, 2);
}

int scene_battle(){
    scene_init("BATTLE");

    m_init(&npc, 1, 1, 100);

    scene_battle_status();
    scene_battle_menu();

    return 0;
}

int shop_buy(){
    if(*shop_foodstock != 0){
        pthread_mutex_lock(&mutex_fsf);
        *shop_foodstock -= 1;
        pthread_mutex_unlock(&mutex_fsf);
        mpp.foodstock++;
    }

    scene_shop_buyer();
}

// int shop_restock(){
//     *shop_foodstock++;

//     scene_shop_merchant();
// }

int scene_shop_buyer_status(){
    mvwprintw(win, 2, 4, "SHOP FOOD STOCK : %03d", *shop_foodstock);
    mvwprintw(win, 3, 4, "YOUR FOOD STOCK : %03d", mpp.foodstock);

    wrefresh(win);
}

int scene_shop_buyer_menu(){
    char *menus[] = {"1. BUY", 
                     "2. BACK"};

    int (*routes[]) () = {shop_buy, scene_idle};

    int (*statuses[]) () = {scene_shop_buyer_status, scene_idle_status};

    menu(menus, routes, statuses, 2);
}


int scene_shop_buyer(){
    scene_init("SHOP (BUYER)");

    scene_shop_buyer_status();
    scene_shop_buyer_menu();

    wrefresh(win);
    getch();
    return 0;
}

// int scene_shop_merchant_status(){
//     mvwprintw(win, 2, 4, "SHOP FOOD STOCK : %03d", *shop_foodstock);

//     wrefresh(win);
// }

// int scene_shop_merchant_menu(){
//     char *menus[] = {"1. RESTOCK", 
//                      "2. BACK"};

//     int (*routes[]) () = {shop_restock, scene_idle};

//     int (*statuses[]) () = {scene_shop_merchant_status, scene_idle_status};

//     menu(menus, routes, statuses, 2);
// }

// int scene_shop_merchant(){
//     scene_init("SHOP (MERCHANT)");

//     scene_shop_merchant_status();
//     scene_shop_merchant_menu();

//     wrefresh(win);
//     getch();
//     return 0;
// }

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