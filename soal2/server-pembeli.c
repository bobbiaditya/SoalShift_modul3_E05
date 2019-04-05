#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8090

int main(int argc, char const *argv[]) {
    // SHARED MEMORY
    key_t key = 1242;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    // SOCKET
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
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

    // Change val

    int new_value = *value;
    char *status[2] = {"transaksi berhasil", "transaksi gagal"};

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

    // // SHARED MEMORY />

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);  

    return 0;
}
