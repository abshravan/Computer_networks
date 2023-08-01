#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERVER_IP "127.0.0.1" // Localhost IP address
#define SERVER_PORT 8888      // Port number to use for communication
#define BUFFER_SIZE 1024      // Size of the data buffer

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server at %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        printf("Enter message to send (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send the message to the server
        send(client_socket, buffer, strlen(buffer), 0);

        // Check if the client wants to exit
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // Receive and display the server's response
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        buffer[bytes_received] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}
