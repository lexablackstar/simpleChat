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

#define MSG_LEN 250

int main (int argc, char* argv[]) {

    if (argc != 2){
        printf ("Usage: ./server <PORT> \n");
        printf ("  You must specify the port for listening \n");
        printf ("  Ex: /server 8080 \n");
        return -1;
    }

    sockaddr_in servaddr, cli;
    int port = atoi (argv[1]);
    char user[20];

    printf ("Please input your username: ");
    scanf ("%s", user);

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
    servaddr.sin_port = htons(port); 

    printf ("Binding... \n");
    if ((bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        return 2; 
    } 

    printf ("Listening on port %d... \n", port);
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
    printf ("Connected: Client %d \n", connfd);

    char buffer[MSG_LEN];
    char msg[MSG_LEN - 20];
    int bytesRead;
    while (true) {
        bzero(buffer, MSG_LEN); 
        bytesRead = recv (connfd, buffer, MSG_LEN, 0);
        if (bytesRead > 0) {
            printf ("%s", buffer);
        }
        bzero(buffer, MSG_LEN);
        bzero(msg, MSG_LEN - 20);
        int n = 0;
        printf ("[%s]: ", user);
        while ((msg[n++] = getchar()) != '\n') {
            // do nothing
        } 
        sprintf (buffer, "[%s]: ", user);
        strcat (buffer, msg);
        if (send (connfd, buffer, strlen(buffer), 0) == -1) 
            printf ("Error sending answer \n");
        
        if (strstr (buffer, "quit"))
            break;
    }

    printf ("Server closed... \n");

    close(sockfd); 

    return 0;
}