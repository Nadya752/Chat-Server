#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when client handler receives invalid messages.
int main(){

    // Client named Ron conencts to Gryffindor.
    char conmsg1[MAX_BUF] = {0};
    connect("Ron", "Gryffindor", conmsg1);
    int fdcon1 = open("gevent", O_WRONLY);
    if (fdcon1 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon1, conmsg1, MAX_BUF);
    close(fdcon1);

    // Client named Harry connects.
    char conmsg2[MAX_BUF] = {0};
    connect("Harry", "Gryffindor", conmsg2);
    int fdcon2 = open("gevent", O_WRONLY);
    if (fdcon2 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon2, conmsg2, MAX_BUF);

    sleep(1);
    char saymsg[MAX_BUF] = {0};

    // Ron sends an invalid NULL char array message.
    ssize_t pwrite = open("./Gryffindor/Ron_WR", O_WRONLY);
    int wrres = write(pwrite,saymsg, MAX_BUF);
    if(wrres == -1){
        fprintf(stderr, "%s", "Can't write pipe ");
    }
    if (pwrite == -1){
        perror("open()");
        fprintf(stderr, "%s", "Can't open write pipe\n");
    }
    close(pwrite);

    // Checks if Ron is set to receive the invalid message of client handler
    // that client handler sends when receiving messages that does not match any
    // message type.
    char newmsg[MAX_BUF] = {0};
    ssize_t pread = open("./Gryffindor/Ron_RD", O_RDONLY);
    if (pread == -1){
        printf("Error opening _RD pipe.\n");

    }
    size_t rd = read(pread,newmsg, MAX_BUF);
    if(rd == -1){
        printf("Error reading.\n");
    }
    

    for(int idx = 0; idx < MAX_BUF; idx++){
        if (newmsg[idx] != 0){
            printf("%c", newmsg[idx]);
        }

    }
    printf("\n");
    
    return 0;

}