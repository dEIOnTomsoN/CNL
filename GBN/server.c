#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sockfd, newsockfd;
    socklen_t clilen;
    int g = 0, n, m, p = 0, f = -1, k;
    char ca = 'a', can = 'c', ch;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(sockfd, 5);
    printf("Server is waiting...\n");

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Accept failed");
        exit(1);
    }

    while (1) {
        n = read(newsockfd, &ch, 1);
        if (n <= 0) break;

        m = read(newsockfd, &g, sizeof(int));
        if (m <= 0) break;

        if (ch == 'd') {
            k = rand();
            if (k % 3 == 0) {
                if (g == f + 1) {
                    printf("Data %d Corrupted\n", g);
                    write(newsockfd, &can, 1);
                    write(newsockfd, &g, sizeof(int));
                    sleep(2);
                } else {
                    printf("Discarded data %d\n", g);
                }
            } else {
                p = f + 1;
                if (g == p) {
                    printf("Data %d received\n", g);
                    write(newsockfd, &ca, 1);
                    write(newsockfd, &g, sizeof(int));
                    printf("Data %d acknowledged\n", g);
                    f = g;
                } else {
                    printf("Discarded data %d\n", g);
                }
                sleep(2);
            }
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

