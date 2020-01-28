#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    struct timeval tv;
    fd_set readfds;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    int ret,fd;
    //fd=open("./test.txt",O_RDWR);

    /*if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } */

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 
    //serv_addr.sin_addr="127.0.0.1";
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 
    while(1)
{	/* Wait on stdin for input. */
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(sockfd, &readfds);

	/* Wait up to five seconds */
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	ret = select (5,&readfds,NULL,NULL,&tv);
	if (ret == -1) {
		perror("select");
		return 1;
	}
if (FD_ISSET(sockfd, &readfds)){
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    if(n < 0)
    {
        printf("\n Read error \n");
    } }
if (FD_ISSET(STDIN_FILENO, &readfds))
{
    	char buf[BUF_LEN];
	int len;

		len = read (STDIN_FILENO, buf, BUF_LEN);
		if (len == -1) {
			perror ("read");
			return 1;
		}
		if (len) {
			buf[len] = '\0';
			printf("read: %s\n", buf);
		}
}
}
return 0;
} 
