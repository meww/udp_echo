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
    int s, count, len;
    struct msg_echo echo;
    struct sockaddr_in myskt;
    in_port_t myport;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: ./udpcli localhost port\n");
        exit(1);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    myport = strtol(argv[2], NULL, 10);
    memset(&myskt, 0, sizeof myskt);
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    inet_aton(argv[1], &myskt.sin_addr);

    echo.seq = 0;
    
    for (;;) {
        printf("message: ");
        if (fgets(echo.msg, sizeof echo.msg, stdin) == NULL) {
            break;
        }
        len = strlen(echo.msg)
        sendto(
    }
}
