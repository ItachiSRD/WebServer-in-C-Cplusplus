#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h> 

#define IP "142.250.70.100"
#define PORT 5353
#define LISTENADDR "0.0.0.0"
/*global Errors*/
char* error;
/*structures*/
struct sHttpRequest{
    char method[8];
    char url[128];
};
typedef struct sHttpRequest httpreq;
int srv_init(int port_no){
    int s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("socket Error !");
        error = "socket Error";
        return 0;
    }
    struct sockaddr_in srv;
    srv.sin_addr.s_addr = inet_addr(LISTENADDR);
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port_no);
    
    if(bind(s, (struct sockaddr *)&srv, sizeof(srv))){
        printf("bind Error !");
        error = "bind Error";
        close(s);
        return 0;
    }
    if(listen(s, 5)){
        printf("listen Error");
        error = "listen Error";
        close(s);
        return 0;
    }
    return s;
}
int cli_accept(int s){
    int c;
    socklen_t addrlen;
    struct sockaddr_in cli;
    memset(&cli, 0, sizeof(cli));
    c = accept(s, (struct sockaddr*)&s, &addrlen); 
    if(c < 0){
        error = "accept() error";
        return 0;
    }
    return c;
}
/*returns 0 on error, or it returns a httpreq structure*/
httpreq* parse_http(char *str){
    httpreq* req;
    req = malloc(sizeof(httpreq));  
    // memset(&req, 0, sizeof(req));
    char* p;
    for(p=str; p != NULL && *p != ' ';p++);
    if(p != NULL && *p == ' '){
        *p = 0;
    }
    else{
        error = "parse_http() NO SPACE Error\n";
        free(req);
        return 0;
    }
    strncpy(req->method, str, 7);

    for(str=++p; p != NULL && *p != ' ';p++);
    if(p != NULL && *p == ' '){
        *p = 0;
    }
    else{
        error = "parse_http() NO SPACE Error\n";
        free(req);
        return 0;
    }
    strncpy(req->url, str, 127);

    return req;

}
/*return 0 on error, or return the data*/
char *cli_read(int c){
    static char buff[512];
    memset(buff, 0, sizeof(buff));
    if(read(c, buff, 511) < 0){
        error = "read() error";
        return 0;
    }
    else{
        return buff;
    }
}
void cli_conn(int s, int c){
    httpreq *req;
    char* p;
    p = cli_read(c);
    if(!p){
        fprintf(stderr, "%s\n", error);
        close(c);
        return;
    }
    req = parse_http(p);
    if(!req){
        fprintf(stderr, "%s\n", error);
        close(c);
        return;
    }
    printf("%s \n %s \n", req->method, req->url);
    free(req);close(c);
    return;
}
int main(int argc, char *argv[]){
    int s, c;
    char *port;

    if(argc < 2){
        fprintf(stderr, "Usage: %s <listening Port>\n", argv[0]);
        return -1;
    }
    else port = argv[1];
    s = srv_init(atoi(port));
    if(!s){
        fprintf(stderr, "%s \n", error);
        close(s);
        return -1;
    }
    printf("listening on %s: %s\n", LISTENADDR, port);
    while(1){
        c = cli_accept(s);
        if(!c){
            fprintf(stderr, "%s\n", error);
            continue;
        }
        printf("Incoming Connection\n");
        if(fork() == 0){
            cli_conn(s, c);
        }
        return -1;
    }
    return 0;
}