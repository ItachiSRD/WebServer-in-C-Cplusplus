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
#define PORT 5353

int main(){
    int serve, clnt;
    socklen_t addrlen; // necessary for accept function
    sockaddr_in srv, cli; // one for the server one for client use to store address
    memset(&srv, 0, sizeof(srv));
    memset(&cli, 0, sizeof(cli));
    char buff[512];
	//    char *data = "";
    serve = socket(AF_INET, SOCK_STREAM, 0);
    if(serve < 0){
        printf("socket() error \n");
        close(serve);
        return -1;
    }
    srv.sin_addr.s_addr = 0;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);

    if(bind(serve, (sockaddr *)&srv, sizeof(srv))){
        printf("bind error \n");
        close(serve);
        return -1;
    }
    if(listen(serve, 5)){
        printf("listen error \n");
        close(serve);
        return -1;
    }
    printf("Listening on 0.0.0.0: %d \n", PORT);
    clnt = accept(serve, (sockaddr *) &srv, &addrlen);
    if(clnt < 0){
        printf("accept failed \n");
        close(serve);
        return -1;
    }
    int n;
    printf("client connected ! \n");
    n = read(clnt, buff, 511);
    char data[] = "httpd v23.0 \n";
    write(clnt, data, strlen(data));
    write(1, buff, n);
    close(clnt), close(serve);
    return 0;
}
 