#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

int sockfd, m, n, s = -1, g = 0;

void* senddata() {
    char ch = 'd';
    while (1) {
        s++;
        write(sockfd, &ch, 1);
        write(sockfd, &s, sizeof(int));
        printf("Send data %d\n", s);
        sleep(1);
    }
}

void* receivedata() {
    char ca;
    while (1) {
        n = read(sockfd, &ca, 1);
        m = read(sockfd, &g, sizeof(int));
        if (n <= 0 || m <= 0) {
            printf("Server disconnected\n");
            exit(1);
        }
        if (ca == 'a') {
            printf("Ack %d Received\n", g);
        } else if (ca == 'c') {
            printf("Sending again\n");
            s = g - 1;
        }
        sleep(1);
    }
}

int main() {
    pthread_t sender, receiver;
    struct sockaddr_in cliaddr;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    cliaddr.sin_family = PF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cliaddr.sin_port = htons(8080);

    if (connect(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0) {
        perror("Connection to server failed");
        exit(1);
    }

    printf("Connected to server\n");

    pthread_create(&sender, NULL, senddata, NULL);
    sleep(1);
    pthread_create(&receiver, NULL, receivedata, NULL);
    sleep(1);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    close(sockfd);
    return 0;
}

