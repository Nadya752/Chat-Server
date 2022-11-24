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
#define NUM_P (3)

// Checks when multiple people wants to SAY at the same time and in between intervals.
int main(){

    // Client named Ron wants to connect.
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

    // Creating and sending the messages of Ron and Harry.
    sleep(3);
    char saymsg[MAX_BUF] = {0};
    say("Ron", saymsg);
    char saymsg2[MAX_BUF] = {0};
    say("Harry", saymsg2);

    // Harry says every one loop and Ron says every 2 loops.
    for ( int idx = 1; idx<=4; idx++){
        if (idx%2 == 0){
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
    }


    // Checks if Hermione
    struct pollfd pipes[NUM_P];
    char newmsg[MAX_BUF] = {0};
    char newmsg1[MAX_BUF] = {0};
    char newmsg2[MAX_BUF] = {0};

    ssize_t pread = open("./Gryffindor/Hermione_RD", O_RDONLY| O_NONBLOCK);
    ssize_t pread1 = open("./Gryffindor/Harry_RD", O_RDONLY| O_NONBLOCK);
    ssize_t pread2 = open("./Gryffindor/Ron_RD", O_RDONLY| O_NONBLOCK);
    char hmsg[] = "HarryHello I'm Harry";
    char rmsg[] = "RonHello I'm Ron";
    int counth = 0;
    int countr = 0;

    // Because messages of Harry (4 message) and Ron (2 message)
    // can arrive in any order, we check and count the number of 
    // Harry's and Ron's messages received by Hermione.
    pipes[0].fd = pread;
    pipes[0].events = POLLIN;
    pipes[1].fd = pread1;
    pipes[1].events = POLLIN;
    pipes[2].fd = pread2;
    pipes[2].events = POLLIN;

    int countmsg = 0;
    while (countmsg <6){
        poll(pipes, NUM_P, 1000);

        short rrevents = pipes[0].revents;
        if (rrevents & POLLIN){
            char stripmsg[MAX_BUF] = {0};
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
            // Checking if it is the right message,
            // and if it is Ron's or Harry's.
            if (strcmp(stripmsg, hmsg) == 0){
                counth++;

            }else if((strcmp(stripmsg, rmsg) == 0)){
                countr++;
            }
            countmsg++;
        }

        short rrevents1 = pipes[1].revents;
        if (rrevents1 & POLLIN){
            int rres = read(pread1,newmsg1, MAX_BUF);
            if(rres == -1){
                fprintf(stderr, "%s", "Can't read pipe ");
            }
        }

        short rrevents2 = pipes[2].revents;
        if (rrevents2 & POLLIN){
            int rres = read(pread2,newmsg2, MAX_BUF);
            if(rres == -1){
                fprintf(stderr, "%s", "Can't read pipe ");
            }
        }
        memset(newmsg, 0,MAX_BUF* sizeof(newmsg[0]));
        memset(newmsg1, 0,MAX_BUF* sizeof(newmsg1[0]));
        memset(newmsg2, 0,MAX_BUF* sizeof(newmsg2[0]));
    }
    close(pread);
    close(pread1);
    close(pread2);

    // If Hermione received the right amount and content of messages
    // from Ron and Harry, "All messages received" will be printed.
    if(countmsg == 6 && countr == 2 && counth == 4){
        printf("All Messages received.\n");
    }else{
        printf("Incomplete messages.\n");
    }

    return 0;

}