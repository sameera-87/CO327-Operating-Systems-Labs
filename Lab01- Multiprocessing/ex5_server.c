#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void handle_client(int sock) {
    char path[BUFFER_SIZE];
    char content[BUFFER_SIZE];
    FILE *fp;

    bzero(path, BUFFER_SIZE);
    read(sock, path, BUFFER_SIZE - 1);

    fp = fopen(path, "r");
    if (fp == NULL) {
        write(sock, "File not found\n", 15);
        close(sock);
        return;
    }
    
    printf("file found");
    // Read and send file line by line
    while (fgets(content, sizeof(content), fp) != NULL) {
        write(sock, content, strlen(content));
    }

    fclose(fp);
    close(sock);
}


int main() {
    int sockfd, newsockfd, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0) {
            // Child
            close(sockfd);
            handle_client(newsockfd);
            exit(0);
        } else {
            // Parent
            close(newsockfd);
            //wait(NULL);  // Wait for child process to finish
        }
    }

    return 0;
}