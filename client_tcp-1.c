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
    int ID, score;
    char Fname[10], Lname[10];
    
    // Receive student information from the client
    recv(newSocket, &ID, sizeof(ID), 0);
    recv(newSocket, Fname, sizeof(Fname), 0);
    recv(newSocket, Lname, sizeof(Lname), 0);
    recv(newSocket, &score, sizeof(score), 0);
    
    // Here, you would add the received student information to the database
    // For simplicity, you can use a text file for the database
    
    // Open the database file in append mode
    FILE *dbFile = fopen("student_database.txt", "a");
    if (dbFile == NULL) {
        perror("Error opening database file");
        exit(EXIT_FAILURE);
    }
    
    // Write student information to the database file
    fprintf(dbFile, "%d %s %s %d\n", ID, Fname, Lname, score);
    
    // Close the database file
    fclose(dbFile);
    
    // Send acknowledgment to the client
    send(newSocket, "Student information added successfully", sizeof("Student information added successfully"), 0);
    break;
}
            }
case 2: {
    // Implement sending student's information based on ID
    int ID;
    // Receive ID from client
    recv(newSocket, &ID, sizeof(ID), 0);
    
    // Open the database file for reading
    FILE *dbFile = fopen("student_database.txt", "r");
    if (dbFile == NULL) {
        perror("Error opening database file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[MAX_BUFFER_SIZE];
    char studentInfo[MAX_BUFFER_SIZE];
    int found = 0; // Flag to indicate if the student with the given ID is found
    
    // Read student information line by line from the database file
    while (fgets(buffer, MAX_BUFFER_SIZE, dbFile) != NULL) {
        // Extract student ID from the buffer
        int currentID;
        sscanf(buffer, "%d", &currentID);
        
        // Check if the current student's ID matches the requested ID
        if (currentID == ID) {
            strcpy(studentInfo, buffer); // Copy the student information to the output buffer
            found = 1; // Set the flag to indicate the student is found
            break;
        }
    }
    
    // Close the database file
    fclose(dbFile);
    
    // Send student information to client
    if (found) {
        send(newSocket, studentInfo, sizeof(studentInfo), 0);
    } else {
        // If student with given ID is not found, send appropriate message to client
        send(newSocket, "Student with specified ID not found", sizeof("Student with specified ID not found"), 0);
    }
    break;
}
case 3: {
    // Implement sending student's information based on score
    int score;
    // Receive score from client
    recv(newSocket, &score, sizeof(score), 0);
    
    // Open the database file for reading
    FILE *dbFile = fopen("student_database.txt", "r");
    if (dbFile == NULL) {
        perror("Error opening database file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[MAX_BUFFER_SIZE];
    char studentInfo[MAX_BUFFER_SIZE];
    int found = 0; // Flag to indicate if any student with score above the given score is found
    
    // Read student information line by line from the database file
    while (fgets(buffer, MAX_BUFFER_SIZE, dbFile) != NULL) {
        // Extract student score from the buffer
        int currentScore;
        sscanf(buffer, "%*d %*s %*s %d", &currentScore);
        
        // Check if the current student's score is above the given score
        if (currentScore > score) {
            strcat(studentInfo, buffer); // Concatenate the student information to the output buffer
            found = 1; // Set the flag to indicate at least one student is found
        }
    }
    
    // Close the database file
    fclose(dbFile);
    
    // Send student information to client
    if (found) {
        send(newSocket, studentInfo, sizeof(studentInfo), 0);
    } else {
        // If no student with score above the given score is found, send appropriate message to client
        send(newSocket, "No student found with score above specified score", sizeof("No student found with score above specified score"), 0);
    }
    break;
}
case 4: {
    // Implement sending all students' information
    // Open the database file for reading
    FILE *dbFile = fopen("student_database.txt", "r");
    if (dbFile == NULL) {
        perror("Error opening database file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[MAX_BUFFER_SIZE];
    char allStudentsInfo[MAX_BUFFER_SIZE] = ""; // Initialize to an empty string
    int studentCount = 0; // Counter to keep track of the number of students
    
    // Read student information line by line from the database file
    while (fgets(buffer, MAX_BUFFER_SIZE, dbFile) != NULL) {
        strcat(allStudentsInfo, buffer); // Concatenate the student information to the output buffer
        studentCount++;
    }
    
    // Close the database file
    fclose(dbFile);
    
    // Send all student information to client
    if (studentCount > 0) {
        send(newSocket, allStudentsInfo, sizeof(allStudentsInfo), 0);
    } else {
        // If no students are found in the database, send appropriate message to client
        send(newSocket, "No students found in the database", sizeof("No students found in the database"), 0);
    }
    break;
}

case 5: {
    // Implement deleting student's information based on ID
    int ID;
    // Receive ID from client
    recv(newSocket, &ID, sizeof(ID), 0);
    
    // Open the database file for reading and writing
    FILE *dbFile = fopen("student_database.txt", "r+");
    if (dbFile == NULL) {
        perror("Error opening database file");
        exit(EXIT_FAILURE);
    }
    
    FILE *tempFile = fopen("temp_database.txt", "w"); // Temporary file for rewriting database
    
    char buffer[MAX_BUFFER_SIZE];
    int deleted = 0; // Flag to indicate if the student with the given ID is deleted
    
    // Read student information line by line from the database file
    while (fgets(buffer, MAX_BUFFER_SIZE, dbFile) != NULL) {
        // Extract student ID from the buffer
        int currentID;
        sscanf(buffer, "%d", &currentID);
        
        // Check if the current student's ID matches the requested ID
        if (currentID != ID) {
            // Write the student information to the temporary file if the ID does not match
            fputs(buffer, tempFile);
        } else {
            // Set the deleted flag if the student is found and deleted
            deleted = 1;
        }
    }
    
    // Close the database file and the temporary file
    fclose(dbFile);
    fclose(tempFile);
    
    // Remove the original database file
    if (remove("student_database.txt") != 0) {
        perror("Error deleting database file");
        exit(EXIT_FAILURE);
    }
    
    // Rename the temporary file to the original database file
    if (rename("temp_database.txt", "student_database.txt") != 0) {
        perror("Error renaming file");
        exit(EXIT_FAILURE);
    }
    
    // Send acknowledgment to the client
    if (deleted) {
        send(newSocket, "Student information deleted successfully", sizeof("Student information deleted successfully"), 0);
    } else {
        send(newSocket, "Student with specified ID not found", sizeof("Student with specified ID not found"), 0);
    }
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
