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

// Checks when multiple people wants to SAY a message.
int main(){

    // Client named Ron wants to connect to Gryffindor.
    char conmsg1[MAX_BUF] = {0};
    connect("Ron", "Gryffindor", conmsg1);
    int fdcon1 = open("gevent", O_WRONLY);
    if (fdcon1 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon1, conmsg1, MAX_BUF);
    close(fdcon1);

    // Client named Harry wants to connect.
    char conmsg2[MAX_BUF] = {0};
    connect("Harry", "Gryffindor", conmsg2);
    int fdcon2 = open("gevent", O_WRONLY);
    if (fdcon2 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon2, conmsg2, MAX_BUF);

    // Client named Hermione wants to connect.
    char conmsg3[MAX_BUF] = {0};
    connect("Hermione", "Gryffindor", conmsg3);
    int fdcon3 = open("gevent", O_WRONLY);
    if (fdcon3 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon3, conmsg3, MAX_BUF);

    // Creating and sending SAY message of Ron.
    sleep(3);
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

    // Creating and sending SAY message of Harry.
    char saymsg2[MAX_BUF] = {0};
    say("Harry", saymsg2);
    ssize_t pwrite2 = open("./Gryffindor/Harry_WR", O_WRONLY);
    int wrres2 = write(pwrite2,saymsg2, MAX_BUF);
    if(wrres2 == -1){
        fprintf(stderr, "%s", "Can't write pipe ");
    }
    if (pwrite2 == -1){
        perror("open()");
        fprintf(stderr, "%s", "Can't open write pipe\n");
    }
    close(pwrite2);

    sleep(1);

    // Checks if Hermione, a client within the domain, receives both
    // of their messages.
    char newmsg[MAX_BUF] = {0};
    char stripmsg[MAX_BUF] = {0};
    char hmsg[] = "HarryHello I'm Harry";
    char rmsg[] = "RonHello I'm Ron";
    int has_hmsg = FALSE;
    int has_rmsg = FALSE;
    ssize_t pread = open("./Gryffindor/Hermione_RD", O_RDONLY);

    // Because messages might arrive in an unpredictable order,
    // Harry's and Ron's message is counted and compared.
    // Ignoring the order of the message, we check if Hermione
    // receives both the messages or not.
    int rres = read(pread,newmsg, MAX_BUF);
    if(rres == -1){
        fprintf(stderr, "%s", "Can't read pipe ");
    }
    int sidx = 0;
    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg[idx] != 0){
            stripmsg[sidx] = newmsg[idx];
            sidx++;
        }

    }
    if (strcmp(stripmsg, hmsg) == 0){
        has_hmsg = TRUE;

    }else if((strcmp(stripmsg, rmsg) == 0)){
        has_rmsg = TRUE;
    }

    sleep(1);
    char newmsg2[MAX_BUF] = {0};
    char stripmsg2[MAX_BUF] = {0};
    int rres2 = read(pread,newmsg2, MAX_BUF);
    if(rres2 == -1){
        fprintf(stderr, "%s", "Can't read pipe ");
    }
    int sidx2 = 0;
    for(int idx = START_PKT; idx < MAX_BUF; idx++){
        if (newmsg2[idx] != 0){
            stripmsg2[sidx2] = newmsg2[idx];
            sidx2++;
        }

    }
    if (strcmp(stripmsg2, hmsg) == 0){
        has_hmsg = TRUE;

    }else if((strcmp(stripmsg2, rmsg) == 0)){
        has_rmsg = TRUE;
    }

    // If Hermione recieves all messsages (Harry's and Ron's), 
    // "All messages recieved" is printed.
    if(has_hmsg && has_rmsg){
        printf("All Messages received.\n");
    }else{
        printf("Incomplete messages.\n");

    }

    return 0;

}