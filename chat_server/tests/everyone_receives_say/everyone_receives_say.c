#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks if everyone in the domain receives SAY message.
int main(){

    // Client named Ron connects to Gryffindor.
    char conmsg1[MAX_BUF] = {0};
    connect("Ron", "Gryffindor", conmsg1);
    int fdcon1 = open("gevent", O_WRONLY);
    if (fdcon1 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon1, conmsg1, MAX_BUF);
    close(fdcon1);

    // Client named Harry connects to Gryffindor.
    char conmsg2[MAX_BUF] = {0};
    connect("Harry", "Gryffindor", conmsg2);
    int fdcon2 = open("gevent", O_WRONLY);
    if (fdcon2 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon2, conmsg2, MAX_BUF);

    // Client named Hermione connects to Gryffindor.
    char conmsg3[MAX_BUF] = {0};
    connect("Hermione", "Gryffindor", conmsg3);
    int fdcon3 = open("gevent", O_WRONLY);
    if (fdcon3 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon3, conmsg3, MAX_BUF);

    // Client named Fred connects to Gryffindor.
    char conmsg4[MAX_BUF] = {0};
    connect("Fred", "Gryffindor", conmsg4);
    int fdcon4 = open("gevent", O_WRONLY);
    if (fdcon4 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon4, conmsg4, MAX_BUF);

    // Client named George connects to Gryffindor.
    char conmsg5[MAX_BUF] = {0};
    connect("George", "Gryffindor", conmsg5);
    int fdcon5 = open("gevent", O_WRONLY);
    if (fdcon5 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon5, conmsg5, MAX_BUF);

    sleep(2);
    // Ron SAYs his message.
    char saymsg[MAX_BUF] = {0};
    say("Ron", saymsg);
    for ( int idx = 0; idx<5; idx++){
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

    }


    sleep(2);
    // Checks if Hermione receives the message.
    char newmsg[MAX_BUF] = {0};
    ssize_t pread = open("./Gryffindor/Hermione_RD", O_RDONLY);
    read(pread,newmsg, MAX_BUF);
    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg[idx] != 0){
            printf("%c", newmsg[idx]);
        }

    }
    close(pread);
    printf("\n");

    // Checks if Harry receives the message.
    char newmsg2[MAX_BUF] = {0};
    ssize_t pread2 = open("./Gryffindor/Harry_RD", O_RDONLY);
    read(pread2,newmsg2, MAX_BUF);
    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg2[idx] != 0){
            printf("%c", newmsg2[idx]);
        }

    }
    close(pread2);
    printf("\n");

    // Checks if Fred receives the message.
    char newmsg3[MAX_BUF] = {0};
    ssize_t pread3 = open("./Gryffindor/Fred_RD", O_RDONLY);
    read(pread3,newmsg3, MAX_BUF);
    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg3[idx] != 0){
            printf("%c", newmsg3[idx]);
        }

    }
    close(pread3);
    printf("\n");

    // Checks if George receives the message.
    char newmsg4[MAX_BUF] = {0};
    ssize_t pread4 = open("./Gryffindor/George_RD", O_RDONLY);
    read(pread4,newmsg4, MAX_BUF);
    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg4[idx] != 0){
            printf("%c", newmsg4[idx]);
        }

    }
    close(pread4);
    printf("\n");

    return 0;

}