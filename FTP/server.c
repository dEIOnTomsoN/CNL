#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void handle_get(int sock_data, char *filename) {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("[Server] Handling GET for file: %s\n", filename);
    
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("[Server] File not found");
        strcpy(buffer, "ERROR: File not found.\n");
        write(sock_data, buffer, strlen(buffer));
        return;
    }

    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        write(sock_data, buffer, n);
    }
    close(fd);
}

void handle_put(int sock_data, char *filename) {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("[Server] Receiving file: %s\n", filename);

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("[Server] Cannot create file");
        return;
    }

    while ((n = read(sock_data, buffer, sizeof(buffer))) > 0) {
        write(fd, buffer, n);
    }
    close(fd);
    printf("[Server] File received successfully!\n");
}

int main(int argc, char *argv[]) {
    int sock_ctrl, sock_data, listen_control, listen_data;
    struct sockaddr_in serv_addr, cli_addr, data_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char buffer[BUFFER_SIZE], cmd[100], filename[100];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listen_control = socket(AF_INET, SOCK_STREAM, 0);
    listen_data = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    data_addr.sin_port = htons(atoi(argv[1]) + 1);

    bind(listen_control, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listen_control, 5);

    bind(listen_data, (struct sockaddr*)&data_addr, sizeof(data_addr));
    listen(listen_data, 5);

    printf("[Server] FTP Server running on port %s...\n", argv[1]);

    while (1) {
        sock_ctrl = accept(listen_control, (struct sockaddr*)&cli_addr, &cli_len);
        if (sock_ctrl < 0) {
            perror("[Server] Accept failed");
            continue;
        }

        printf("[Server] Client connected!\n");

        while (1) {
            bzero(cmd, sizeof(cmd));
            read(sock_ctrl, cmd, sizeof(cmd) - 1);

            if (strcmp(cmd, "get") == 0) {
                read(sock_ctrl, filename, sizeof(filename));
                sock_data = accept(listen_data, (struct sockaddr*)&cli_addr, &cli_len);
                handle_get(sock_data, filename);
                close(sock_data);
            } else if (strcmp(cmd, "put") == 0) {
                read(sock_ctrl, filename, sizeof(filename));
                sock_data = accept(listen_data, (struct sockaddr*)&cli_addr, &cli_len);
                handle_put(sock_data, filename);
                close(sock_data);
            } else if (strcmp(cmd, "bye") == 0) {
                printf("[Server] Client disconnected.\n");
                break;
            }
        }

        close(sock_ctrl);
    }

    return 0;
}

