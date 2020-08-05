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

    if (argc != 3){
        printf ("Usage: ./client <IP> <PORT> \n");
        printf ("  You must specify the ip and the port of the server \n");
        printf ("  Ex: /client 127.0.0.1 8080 \n");
        return -1;
    }

    sockaddr_in servaddr, cli;

    char ip[16];
    strcpy (ip, argv[1]);
    int port = atoi (argv[2]);
    char user[20];

    printf ("Please input your username: ");
    scanf ("%s", user);

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
    servaddr.sin_addr.s_addr = inet_addr(ip); 
    servaddr.sin_port = htons(port); 

    printf ("Connecting to %s: %d... \n", ip, port);
    if ((connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Connection failed...\n"); 
        return 2; 
    } 
    printf ("Connected to server \n");

    char buffer[MSG_LEN];
    int bytesRead;
    char msg[MSG_LEN - 20];
    while (true) {

        bzero(buffer, MSG_LEN);
        bzero(msg, MSG_LEN - 20);
        int n = 0;
        printf ("[%s]: ", user);
        while ((msg[n++] = getchar()) != '\n') {
            // do nothing
        } 
        sprintf (buffer, "[%s]: ", user);
        strcat (buffer, msg);
        if (send (sockfd, buffer, strlen(buffer), 0) == -1) 
            printf ("Error sending message \n");

        bzero(buffer, MSG_LEN); 
        bytesRead = recv (sockfd, buffer, MSG_LEN, 0);
        if (bytesRead > 0) {
            printf ("%s", buffer);
        }


        if (strstr (buffer, "quit"))
            break;
    }

    printf ("Client closed... \n");

    close(sockfd); 

    return 0;
}