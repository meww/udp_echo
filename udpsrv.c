#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct msg_echo {
    unsigned short seq;
    unsigned short reserve;
    char msg[32];
};

int main(int argc, char *argv[])
{
    int s, count, servlen;
    socklen_t recvlen;
    in_port_t myport;
    struct sockaddr_in servskt, recvskt;
    struct msg_echo echo;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: ./udpsrv port\n");
        exit(1);
    }
    
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    myport = strtol(argv[1], NULL, 10);
    memset(&servskt, 0, sizeof servskt);
    servskt.sin_family = AF_INET;
    servskt.sin_port = htons(myport);
    servskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&servskt, sizeof servskt) < 0) {
        perror("bind");
        exit(1);
    }
    
    for (;;) {
        recvlen = sizeof recvskt;
        if ((count = recvfrom(s, &echo, sizeof echo, 0, 
                    (struct sockaddr*)&recvskt, &recvlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("%s\n", echo.msg);
        echo.seq++;
        echo.msg[count - sizeof(unsigned short) * 2] = '\0';
        servlen = strlen(echo.msg) + sizeof(unsigned short) * 2;
        if ((count = sendto(s, &echo, servlen, 0,
                    (struct sockaddr *)&recvskt, recvlen)) < 0) {
            perror("sendto");
            exit(1);
        }
    }
    close(s);

    return 0;
}
