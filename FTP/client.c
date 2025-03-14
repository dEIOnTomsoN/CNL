#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock_ctrl, sock_data, fd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE], cmd[100], filename[100];
    ssize_t n;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    sock_ctrl = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sock_ctrl, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("[Client] Connection failed");
        exit(1);
    }

    while (1) {
        printf("Enter command (get <filename> / put <filename> / bye): ");
        scanf("%s", cmd);

        if (strcmp(cmd, "bye") == 0) {
            write(sock_ctrl, cmd, strlen(cmd));
            break;
        }

        scanf("%s", filename);
        write(sock_ctrl, cmd, strlen(cmd));
        write(sock_ctrl, filename, strlen(filename));

        serv_addr.sin_port = htons(atoi(argv[1]) + 1);
        sock_data = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sock_data, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("[Client] Data connection failed");
            continue;
        }

        if (strcmp(cmd, "get") == 0) {
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("[Client] Error opening file");
                close(sock_data);
                continue;
            }

            while ((n = read(sock_data, buffer, sizeof(buffer))) > 0) {
                write(fd, buffer, n);
            }
            close(fd);
            printf("[Client] File %s downloaded successfully.\n", filename);
        } 
        else if (strcmp(cmd, "put") == 0) {
            fd = open(filename, O_RDONLY);
            if (fd < 0) {
                perror("[Client] Error opening file");
                close(sock_data);
                continue;
            }

            while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
                write(sock_data, buffer, n);
            }
            close(fd);
            printf("[Client] File %s uploaded successfully.\n", filename);
        }

        close(sock_data);
    }

    close(sock_ctrl);
    return 0;
}

