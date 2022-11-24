#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when a client wants to connect to a nested domain.
int main(){

    // Client named Ron wants to connect to domain Gryffindor.
    char conmsg1[MAX_BUF] = {0};
    check_gevent();
    connect("Ron", "Gryffindor", conmsg1);
    int fdcon1 = open("gevent", O_WRONLY);
    if (fdcon1 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon1, conmsg1, MAX_BUF);
    close(fdcon1);
    sleep(1);
    check_domain("Gryffindor"); // Checks if domain Gryffindor exists.
    check_rd_wr_pipe("Gryffindor", "Ron_RD", "Ron_WR");

    // Client named Harry wants to connect to domain Gryffindor.
    char conmsg2[MAX_BUF] = {0};
    connect("Harry", "Gryffindor", conmsg2);
    int fdcon2 = open("gevent", O_WRONLY);
    if (fdcon2 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon2, conmsg2, MAX_BUF);
    close(fdcon2);
    sleep(1);
    check_rd_wr_pipe("Gryffindor", "Harry_RD", "Harry_WR");

    // Client named Hermione wants to connect to domain Gryffindor.    
    char conmsg3[MAX_BUF] = {0};
    connect("Hermione", "Gryffindor", conmsg3);
    int fdcon3 = open("gevent", O_WRONLY);
    if (fdcon3 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon3, conmsg3, MAX_BUF);
    close(fdcon3);
    sleep(1);
    check_rd_wr_pipe("Gryffindor", "Hermione_RD", "Hermione_WR");

    // Client named Fred wants to connect to domain Gryffindor.    
    char conmsg4[MAX_BUF] = {0};
    connect("Fred", "Gryffindor", conmsg4);
    int fdcon4 = open("gevent", O_WRONLY);
    if (fdcon4 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon4, conmsg4, MAX_BUF);
    close(fdcon4);
    sleep(1);
    check_rd_wr_pipe("Gryffindor", "Fred_RD", "Fred_WR");

    // Client named George wants to connect to domain Gryffindor.    
    char conmsg5[MAX_BUF] = {0};
    connect("George", "Gryffindor", conmsg5);
    int fdcon5 = open("gevent", O_WRONLY);
    if (fdcon5 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon5, conmsg5, MAX_BUF);
    close(fdcon5);
    sleep(1);
    check_rd_wr_pipe("Gryffindor", "George_RD", "George_WR");

    // Ron wants to connect to domain named Golden Trio, 
    // which is a part of Gryffindor.
    char conmsg6[MAX_BUF] = {0};
    connect("Ron", "Gryffindor/Golden Trio", conmsg6);
    int fdcon6 = open("gevent", O_WRONLY);
    if (fdcon6 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon6, conmsg6, MAX_BUF);
    close(fdcon6);
    sleep(1);
    check_domain("Gryffindor/Golden Trio"); // Checks if Golden Trio domain exists.
    check_rd_wr_pipe("Gryffindor/Golden Trio", "Ron_RD", "Ron_WR");

    // Harry wants to connect to Golden Trio.
    char conmsg7[MAX_BUF] = {0};
    connect("Harry", "Gryffindor/Golden Trio", conmsg7);
    int fdcon7 = open("gevent", O_WRONLY);
    if (fdcon7 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon7, conmsg7, MAX_BUF);
    close(fdcon7);
    sleep(1);
    check_rd_wr_pipe("Gryffindor/Golden Trio", "Harry_RD", "Harry_WR");

    // Hermione wants to connect to Golden Trio.
    char conmsg8[MAX_BUF] = {0};
    connect("Hermione", "Gryffindor/Golden Trio", conmsg8);
    int fdcon8 = open("gevent", O_WRONLY);
    if (fdcon8 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon8, conmsg8, MAX_BUF);
    close(fdcon8);
    sleep(1);
    check_rd_wr_pipe("Gryffindor/Golden Trio", "Hermione_RD", "Hermione_WR");

    return 0;

}
