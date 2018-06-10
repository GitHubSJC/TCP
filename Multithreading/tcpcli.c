#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != 0);

    struct sockaddr_in ser, cli;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(6000);
    ser.sin_addr.s_addr = inet_addr("192.168.1.145");

    int c = connect(sockfd, (struct sockaddr*) & ser, sizeof(ser));
    assert(c != -1);

    while(1)
    {
        printf("输入:");
        char buf[128];
        fgets(buf, 128, stdin);

        ssize_t s = send(sockfd, buf, strlen(buf) - 1, 0);
        assert(s != -1);

        if(strncmp(buf, "end", 3) == 0)
        {
            close(sockfd);
            break;
        }
        char buff[32] = {0};
        ssize_t r = recv(sockfd, buff, sizeof(buff), 0);
        printf("..%s\n", buff);
    }
    close(sockfd);
}
