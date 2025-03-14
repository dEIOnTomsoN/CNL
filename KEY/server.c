#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 255

struct KeyValue {
    int key;
    char value[BUFFER_SIZE];
};

int main() {
    int server_fd, client_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char buffer[BUFFER_SIZE];
    struct KeyValue kv_store[] = {
        {101, "Apple"}, {102, "Banana"}, {103, "Orange"}, {104, "Mango"}, {105, "Grapes"}
    };
    int num_entries = sizeof(kv_store) / sizeof(kv_store[0]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Key-Value Store Server is running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*)&cli_addr, &cli_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t n = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[n] = '\0';
        int key = atoi(buffer);
        printf("Received key: %d\n", key);
        char result[BUFFER_SIZE] = "Key not found";

        for (int i = 0; i < num_entries; i++) {
            if (kv_store[i].key == key) {
                strcpy(result, kv_store[i].value);
                break;
            }
        }

        write(client_fd, result, strlen(result));
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

