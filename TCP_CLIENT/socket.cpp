#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#define IP "142.250.70.100"
#define PORT 80
using namespace std;

int main(){
    int s;
    // structural address to put our ip addreeses, etc
    sockaddr_in sock;
    // 
    char buff[512];
    char data[] =  "HEAD / HTTP/1.0 \n\n";
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("socket() error \n");
        return -1;
    }
    sock.sin_addr.s_addr = inet_addr(IP);
    sock.sin_port = htons(PORT);
    sock.sin_family = AF_INET;
    if(connect(s, (sockaddr *)&sock, sizeof(sockaddr_in)) != 0){
        printf("Connection Error! \n");
        close(s);
        return -1;
    }
    printf("HI");
    write(s, data, strlen(data));
    read(s, buff, 511);
    close(s);
    printf("\n %s \n", buff);
    return 0;
}