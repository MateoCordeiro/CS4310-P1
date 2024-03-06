/***************Server program**************************/

/* server_tcp.c is on eros.cs.txstate.edu
   open a window on eros.
   use a port number between 10,000-15,000.
   compile and run the server program first:
   $gcc -o s server_tcp.c
   $./s 12000
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int welcomeSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    unsigned short port;
    int choice;

    // Create socket
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (welcomeSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    port = (unsigned short)atoi(argv[1]);
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Bind socket
    if (bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(welcomeSocket, 5) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    // Accept incoming connections
    addr_size = sizeof clientAddr;
    newSocket = accept(welcomeSocket, (struct sockaddr *)&clientAddr, &addr_size);
    if (newSocket < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    // Communication with client
    do {
        // Receive client's choice
        recv(newSocket, &choice, sizeof(choice), 0);

        // Handle different choices
        switch (choice) {
            case 1: {
                // Implement adding student information to the database
                break;
            }
            case 2: {
                // Implement sending student's information based on ID
                break;
            }
            case 3: {
                // Implement sending student's information based on score
                break;
            }
            case 4: {
                // Implement sending all students' information
                break;
            }
            case 5: {
                // Implement deleting student's information based on ID
                break;
            }
            case 6: {
                printf("Client requested to exit.\n");
                break;
            }
            default:
                printf("Invalid choice received.\n");
        }
    } while (choice != 6);

    // Close sockets
    close(newSocket);
    close(welcomeSocket);

    return 0;
}
