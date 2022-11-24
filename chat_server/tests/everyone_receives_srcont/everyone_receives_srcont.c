#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks if everyone in the domain receives the SAYRECVCONT messages.
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

    // Client named Harry connects.
    char conmsg2[MAX_BUF] = {0};
    connect("Harry", "Gryffindor", conmsg2);
    int fdcon2 = open("gevent", O_WRONLY);
    if (fdcon2 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon2, conmsg2, MAX_BUF);

    // Client named Hermione connects.
    char conmsg3[MAX_BUF] = {0};
    connect("Hermione", "Gryffindor", conmsg3);
    int fdcon3 = open("gevent", O_WRONLY);
    if (fdcon3 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon3, conmsg3, MAX_BUF);

    // Client named Fred connects.
    char conmsg4[MAX_BUF] = {0};
    connect("Fred", "Gryffindor", conmsg4);
    int fdcon4 = open("gevent", O_WRONLY);
    if (fdcon4 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon4, conmsg4, MAX_BUF);

    // Client named George connects.
    char conmsg5[MAX_BUF] = {0};
    connect("George", "Gryffindor", conmsg5);
    int fdcon5 = open("gevent", O_WRONLY);
    if (fdcon5 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon5, conmsg5, MAX_BUF);

    sleep(2);
    // Creating and sending the SAYCONT messages of Ron.
    char contmsg1[MAX_BUF] = {0};
    char contmsg2[MAX_BUF] = {0};
    char contmsg3[MAX_BUF] = {0};
    saycont("Ron", "Gryffindor", contmsg1, 0);
    saycont("Ron", "Gryffindor", contmsg2, 1);
    saycont("Ron", "Gryffindor", contmsg3, 2);

    ssize_t pwrite = open("./Gryffindor/Ron_WR", O_WRONLY);
    int wrres = write(pwrite,contmsg1, MAX_BUF);
    if(wrres == -1){
        fprintf(stderr, "%s", "Can't write pipe ");
    }
    if (pwrite == -1){
        perror("open()");
        fprintf(stderr, "%s", "Can't open write pipe\n");
    }
    close(pwrite);
    
    ssize_t pwrite2 = open("./Gryffindor/Ron_WR", O_WRONLY);
    int wrres2 = write(pwrite2,contmsg2, MAX_BUF);
    if(wrres2 == -1){
        fprintf(stderr, "%s", "Can't write pipe ");
    }
    if (pwrite2 == -1){
        perror("open()");
        fprintf(stderr, "%s", "Can't open write pipe\n");
    }
    close(pwrite2);

    ssize_t pwrite3 = open("./Gryffindor/Ron_WR", O_WRONLY);
    int wrres3 = write(pwrite3,contmsg3, MAX_BUF);
    if(wrres3 == -1){
        fprintf(stderr, "%s", "Can't write pipe ");
    }
    if (pwrite3 == -1){
        perror("open()");
        fprintf(stderr, "%s", "Can't open write pipe\n");
    }
    close(pwrite3);

    // Checks if Harry receives the RECVCONT messages.
    sleep(2);
    char newmsg[MAX_BUF] = {0};
    for (int i = 0; i<3; i++){
        ssize_t pread = open("./Gryffindor/Harry_RD", O_RDONLY);
        read(pread,newmsg, MAX_BUF);
        for(int idx = START_PKT; idx < MAX_BUF; idx++){
            if (newmsg[idx] != 0){
                if (idx == MAX_BUF-1){
                    printf("%x", newmsg[idx]);
                }else{
                    printf("%c", newmsg[idx]);
                }

            }

        }
        printf("\n");
        memset(newmsg, 0,MAX_BUF* sizeof(newmsg[0]));
        close(pread);
    }

    // Checks if Hermione receives the RECVCONT messages.
    char newmsg2[MAX_BUF] = {0};
    for (int i = 0; i<3; i++){
        ssize_t pread = open("./Gryffindor/Hermione_RD", O_RDONLY);
        read(pread,newmsg2, MAX_BUF);
        for(int idx = START_PKT; idx < MAX_BUF; idx++){
            if (newmsg2[idx] != 0){
                if (idx == MAX_BUF-1){
                    printf("%x", newmsg2[idx]);
                }else{
                    printf("%c", newmsg2[idx]);
                }

            }

        }
        printf("\n");
        memset(newmsg2, 0,MAX_BUF* sizeof(newmsg2[0]));
        close(pread);
    }

    // Checks if Fred receives the RECVCONT messages.
    char newmsg3[MAX_BUF] = {0};
    for (int i = 0; i<3; i++){
        ssize_t pread = open("./Gryffindor/Fred_RD", O_RDONLY);
        read(pread,newmsg3, MAX_BUF);
        for(int idx = START_PKT; idx < MAX_BUF; idx++){
            if (newmsg3[idx] != 0){
                if (idx == MAX_BUF-1){
                    printf("%x", newmsg3[idx]);
                }else{
                    printf("%c", newmsg3[idx]);
                }

            }

        }
        printf("\n");
        memset(newmsg3, 0,MAX_BUF* sizeof(newmsg3[0]));
        close(pread);
    }

    // Checks if George receives the RECVCONT messages.
    char newmsg4[MAX_BUF] = {0};
    for (int i = 0; i<3; i++){
        ssize_t pread = open("./Gryffindor/George_RD", O_RDONLY);
        read(pread,newmsg4, MAX_BUF);
        for(int idx = START_PKT; idx < MAX_BUF; idx++){
            if (newmsg4[idx] != 0){
                if (idx == MAX_BUF-1){
                    printf("%x", newmsg4[idx]);
                }else{
                    printf("%c", newmsg4[idx]);
                }

            }

        }
        printf("\n");
        memset(newmsg4, 0,MAX_BUF* sizeof(newmsg4[0]));
        close(pread);
    }

    return 0;

}