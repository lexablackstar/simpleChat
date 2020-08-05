#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080 

int main () {

    sockaddr_in servaddr, cli;

    printf ("Client started :) \n");
    printf ("Opening the socket... \n");
    int sockfd = socket( AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf ("Socket error \n");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 

    printf ("Connecting to 127.0.0.1:8080... \n");
    if ((connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Connection failed...\n"); 
        return 2; 
    } 

    printf ("Connected to server \n");

    char buffer[250];
    int bytesRead;
    while (true) {

        bzero(buffer, 250);
        int n = 0;
        printf ("Message: ");
        while ((buffer[n++] = getchar()) != '\n') {
            // do nothing
        } 
        if (send (sockfd, buffer, strlen(buffer), 0) == -1) 
            printf ("Error sending message \n");

        bzero(buffer, 250); 
        bytesRead = recv (sockfd, buffer, 250, 0);
        if (bytesRead > 0) {
            printf ("Server answer (%d bytes) : %s \n", bytesRead, buffer);
        }


        if (strstr (buffer, "exit"))
            break;
    }

    printf ("Client closed... \n");

    close(sockfd); 

    return 0;
}