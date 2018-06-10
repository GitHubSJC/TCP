#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void Pthread(void *arg)
{
    ssize_t clifd = *(ssize_t*)arg;
    while(1)
    {
        char buff[128] = {0};
        ssize_t r = recv(clifd, buff, sizeof(buff) - 1, 0);
        if(r <= 0)
        {
            close(clifd);
            break;
        }
        if(strncmp(buff, "end", 3) == 0)
        {
            close(clifd);
            break;
        }
        printf("%s\n", buff);
        //printf("%d:  %s\n", clifd, buff);

        send(clifd, "OK", 2, 0);
    }
    pthread_exit(NULL);
}

int main()
{
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd != -1);

    struct sockaddr_in ser, cli;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(6000);
    ser.sin_addr.s_addr = INADDR_ANY;

    int res = bind(listenfd, (struct sockaddr*)&ser, sizeof(ser));
    assert(res != -1);

    int lis = listen(listenfd, 5);
    assert(lis != -1);

    while(1)
    {
        int len = sizeof(cli);
        int clifd = accept(listenfd, (struct sockaddr*)&cli, &len);
        assert(clifd != -1);
        pthread_t pth;
        pthread_create(&pth, NULL, (void*)Pthread, &clifd);
    }

    close(listenfd);
}
