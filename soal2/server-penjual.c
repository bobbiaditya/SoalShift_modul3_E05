#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#define PORT 8080
#define FOR(i, k) for(int i=0; i<k; i++)

pthread_t tid[2];
int shmid;
int *value;

void connect_sm(){
    key_t key = 1242;

    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    return;
}

void socket_server(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

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
    // else if(pthread_equal(id, tid[1])){
    //     while(1){
    //         socket_server();
    //     }
    // }
}

int main(int argc, char const *argv[]) {
    // SHARED MEMORY
    connect_sm();

    // MULTITHREAD
    int err;
    FOR(i, 1){
        err = pthread_create(&(tid[i]), NULL, &multr, NULL);
        
        if(err != 0){
            printf("Can't create thread : [%s]\n", strerror(err));
        } else {
            printf("Created thread successfully\n");
        }
    }

    socket_server();

    // // SHARED MEMORY />

    // shmdt(value);
    // shmctl(shmid, IPC_RMID, NULL);  

    return 0;
}
