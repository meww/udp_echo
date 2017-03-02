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
    struct msg_echo echo;
    struct sockaddr_in servskt, recvskt;
    in_port_t myport;
    socklen_t recvlen;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: ./udpcli localhost port\n");
        exit(1);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    myport = strtol(argv[2], NULL, 10);
    memset(&servskt, 0, sizeof servskt);
    servskt.sin_family = AF_INET;
    servskt.sin_port = htons(myport);
    inet_aton(argv[1], &servskt.sin_addr);

    echo.seq = 0;
    
    for (;;) {
        printf("message: ");
        if (fgets(echo.msg, sizeof echo.msg, stdin) == NULL) {
            break;
        }
        echo.msg[strlen(echo.msg) - 1] = '\0';
        servlen = strlen(echo.msg) + sizeof(unsigned short) * 2;

        if (strcmp(echo.msg, "FIN") == 0) {
            printf("Finished\n");
            break;
        } else if (echo.seq == 10) {
            printf("seq = 10\n");
            break;
        }
        if ((count = sendto(s, &echo, servlen, 0, 
                    (struct sockaddr *)&servskt, sizeof servskt)) < 0) {
            perror("sendto");
            exit(1);
        }
        recvlen = sizeof recvskt;
        if ((count = recvfrom(s, &echo, sizeof echo, 0,
                    (struct sockaddr *)&recvskt, &recvlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        echo.msg[count - sizeof(unsigned short) * 2] = '\0';
        printf("seq: %d, msg: %s\n", echo.seq, echo.msg);
    }
    close(s);

    return 0;
}
