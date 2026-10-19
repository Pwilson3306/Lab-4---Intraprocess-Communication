#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9001
#define BUFFER_SIZE 1024

int main(void) {
    int sock;
    struct sockaddr_in server_address;
    char command[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &server_address.sin_addr) != 1) {
        fprintf(stderr, "Invalid server address\n");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    printf("Connected. Enter commands (type exit to quit).\n");

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (command[0] == '\0') {
            continue;
        }

        size_t command_length = strlen(command);

        if (send(sock, command, command_length, 0) < 0) {
            perror("send");
            break;
        }

        if (strcmp(command, "exit") == 0) {
            break;
        }

        memset(response, 0, sizeof(response));

        ssize_t received = recv(sock, response,
                                sizeof(response) - 1, MSG_WAITALL);

        if (received <= 0) {
            fprintf(stderr, "\nServer disconnected or response failed.\n");
            break;
        }

        response[received] = '\0';
        printf("%s\n", response);
    }

    close(sock);
    return 0;
}
