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

// Checks when a client SAYCONTs a message.
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

    sleep(1);
    // Creating SAYCONT messages for Ron.
    char contmsg1[MAX_BUF] = {0};
    char contmsg2[MAX_BUF] = {0};
    char contmsg3[MAX_BUF] = {0};
    saycont("Ron", "Gryffindor", contmsg1, 0);
    saycont("Ron", "Gryffindor", contmsg2, 1);
    saycont("Ron", "Gryffindor", contmsg3, 2);

    // Ron sends SAYCONTs messages.
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

    sleep(3);
    // Checks if Harry recieves all the SAYCONTs messages, including the termination byte.
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

    return 0;
    
}