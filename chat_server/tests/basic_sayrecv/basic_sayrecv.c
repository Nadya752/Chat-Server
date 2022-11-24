#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <poll.h>

// Checks when a client SAYs a message in a domain.
int main(){

    // Client named Ron connects.
    char conmsg1[MAX_BUF] = {0};
    connect("Ron", "Gryffindor", conmsg1);
    int fdcon1 = open("gevent", O_WRONLY);
    if (fdcon1 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon1, conmsg1, MAX_BUF);
    close(fdcon1);

    // Client named Harry also connects.
    char conmsg2[MAX_BUF] = {0};
    connect("Harry", "Gryffindor", conmsg2);
    int fdcon2 = open("gevent", O_WRONLY);
    if (fdcon2 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon2, conmsg2, MAX_BUF);

    sleep(1);

    // Creating the SAY message for Ron.
    char saymsg[MAX_BUF] = {0};
    say("Ron", saymsg);
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

    // Checks if Harry receives Ron's message.
    char newmsg[MAX_BUF] = {0};
    ssize_t pread = open("./Gryffindor/Harry_RD", O_RDONLY);
    read(pread,newmsg, MAX_BUF);

    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg[idx] != 0){
            printf("%c", newmsg[idx]);
        }

    }
    printf("\n");

    return 0;

}