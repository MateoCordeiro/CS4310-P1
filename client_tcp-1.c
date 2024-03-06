/****************Client program********************************/

/* client_tcp.c is on zeus.cs.txstate.edu
   open a window on zeus.
   use a port number between 10,000-15,000.
   compile:
   $gcc -o c client_tcp.c
   $./c eros.cs.txstate.edu 12000
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int clientSocket;
    struct sockaddr_in serverAddr;
    struct hostent *hostnm;
    unsigned short port;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    port = (unsigned short)atoi(argv[2]);
    serverAddr.sin_port = htons(port);
    hostnm = gethostbyname(argv[1]);
    if (hostnm == NULL) {
        fprintf(stderr, "Error: no such host\n");
        exit(EXIT_FAILURE);
    }
    serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Communication with server
    int choice;
    do {
        printf("Menu:\n");
        printf("1. add(ID, Fname, Lname, score)\n");
        printf("2. display(ID)\n");
        printf("3. display(score)\n");
        printf("4. display_all\n");
        printf("5. delete(ID)\n");
        printf("6. exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Send choice to server
        send(clientSocket, &choice, sizeof(choice), 0);

// Handle different choices
switch (choice) {
    case 1: {
        // Add student information
        printf("Enter student ID, First Name, Last Name, and Score: ");
        int ID, score;
        char Fname[10], Lname[10];
        scanf("%d %s %s %d", &ID, Fname, Lname, &score);
        send(clientSocket, &ID, sizeof(ID), 0);
        send(clientSocket, Fname, sizeof(Fname), 0);
        send(clientSocket, Lname, sizeof(Lname), 0);
        send(clientSocket, &score, sizeof(score), 0);
        recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        printf("Server response: %s\n", buffer);
        break;
    }
    case 2: {
        // Display student information by ID
        printf("Enter student ID: ");
        int ID;
        scanf("%d", &ID);
        send(clientSocket, &ID, sizeof(ID), 0);
        recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        printf("Student information: %s\n", buffer);
        break;
    }
    case 3: {
        // Display student information by score
        printf("Enter score: ");
        int score;
        scanf("%d", &score);
        send(clientSocket, &score, sizeof(score), 0);
        recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        printf("Students with score above %d: %s\n", score, buffer);
        break;
    }
    case 4: {
        // Display all student information
        send(clientSocket, &choice, sizeof(choice), 0);
        recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        printf("All student information:\n%s\n", buffer);
        break;
    }
    case 5: {
        // Delete student information by ID
        printf("Enter student ID to delete: ");
        int ID;
        scanf("%d", &ID);
        send(clientSocket, &ID, sizeof(ID), 0);
        recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        printf("Server response: %s\n", buffer);
        break;
    }
    case 6: {
        printf("Exiting program...\n");
        break;
    }
    default:
        printf("Invalid choice. Please try again.\n");
}

    } while (choice != 6);

    // Close socket
    close(clientSocket);

    return 0;
}
