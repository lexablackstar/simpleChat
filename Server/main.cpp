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

    printf ("Opening the socket... \n");
    int sockfd = socket( AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf ("Socket error \n");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 

    printf ("Binding on port 8080... \n");
    if ((bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        return 2; 
    } 

    printf ("Listening... \n");
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        return 3; 
    } 

    socklen_t len = sizeof(cli);
    int connfd = accept(sockfd, (sockaddr*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        return 4; 
    } 
    printf ("Connected: Socket %d \n", connfd);

    char buffer[250];
    int bytesRead;
    while (true) {
        bzero(buffer, 250); 
        bytesRead = recv (connfd, buffer, 250, 0);
        if (bytesRead > 0) {
            printf ("Socket %d (%d bytes) : %s \n", connfd, bytesRead, buffer);
        }
        bzero(buffer, 250);
        int n = 0;
        printf ("Your answer: ");
        while ((buffer[n++] = getchar()) != '\n') {
            // do nothing
        } 
        if (send (connfd, buffer, strlen(buffer), 0) == -1) 
            printf ("Error sending answer \n");
        
        if (strstr (buffer, "exit"))
            break;
    }

    printf ("Server closed... \n");

    close(sockfd); 

    return 0;
}