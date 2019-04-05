#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define FOR(i, k) for(int i=0; i<k; i++)
#define FORR(i, j, k) for(int i=j; i<=k; i++)
#define HEIGHT 24
#define WIDTH 64

static int counter = 0;
const int pos_start[2] = {0, 1};
int *shop_foodstock = 0;
key_t key = 4242;

pthread_mutex_t mutex_sc = PTHREAD_MUTEX_INITIALIZER;

WINDOW * win;
int (*scene_current_status)();

int shop_restock();

void clear_status(){
    FOR(i, 6){
        mvwprintw(win, 2+i, 4, "                                            ");
    }
}

void scene_init(char * title){
    werase(win);

    // Window
    box(win, 0, 0);
    mvwprintw(win,  0, 2, "[ %s SCENE ]", title);
    refresh();
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

int scene_die_status(){
    clear_status();

    mvwprintw(win, 10, 4, "YOUR MONSTER DIE!");
    mvwprintw(win, 12, 4, "Press x button to exit...");

    wrefresh(win);
}

int scene_die(){
    // scene_init("MONSTER DIE");

    // scene_die_status();

    // while(getch() != 'x');

    endwin();
    curs_set(1);
    exit(0);
}

int scene_shop_merchant_status(){
    mvwprintw(win, 2, 4, "SHOP FOOD STOCK : %03d", *shop_foodstock);

    refresh();
}

int scene_shop_merchant_menu(){
    char *menus[] = {"1. RESTOCK", 
                     "2. EXIT"};

    int (*routes[]) () = {shop_restock, scene_die};

    int (*statuses[]) () = {scene_shop_merchant_status, scene_die_status};

    menu(menus, routes, statuses, 2);
}

int scene_shop_merchant(){
    scene_init("SHOP (MERCHANT)");

    scene_shop_merchant_status();
    scene_shop_merchant_menu();

    refresh();

    wrefresh(win);
    getch();
    return 0;
}

int main(){
    // SHM
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shop_foodstock = shmat(shmid, NULL, 0);  

    // NCURSES
    initscr();
    noecho();

    win  = newwin(HEIGHT, WIDTH, pos_start[0], pos_start[1]);

    scene_shop_merchant();

    endwin();
}

int shop_restock(){
    *shop_foodstock += 1;

    scene_shop_merchant();
}