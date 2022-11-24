#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when clients connects to multiple domains.
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
    check_domain("Gryffindor"); // Checks if Gryffindor domain exists.
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

    // Client named Cho wants to connect to domain Ravenclaw.
    char conmsg4[MAX_BUF] = {0};
    connect("Cho", "Ravenclaw", conmsg4);
    int fdcon4 = open("gevent", O_WRONLY);
    if (fdcon4 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon4, conmsg4, MAX_BUF);
    close(fdcon4);
    sleep(1);
    check_domain("Ravenclaw"); // Checks if Ravenclaw domain exists.
    check_rd_wr_pipe("Ravenclaw", "Cho_RD", "Cho_WR");

    // Client named Luna wants to connect to domain Ravenclaw.
    char conmsg5[MAX_BUF] = {0};
    connect("Luna", "Ravenclaw", conmsg5);
    int fdcon5 = open("gevent", O_WRONLY);
    if (fdcon5 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon5, conmsg5, MAX_BUF);
    close(fdcon5);
    sleep(1);
    check_rd_wr_pipe("Ravenclaw", "Luna_RD", "Luna_WR");

    // Client named Draco wants to connect to domain Slytherin.
    char conmsg6[MAX_BUF] = {0};
    connect("Draco", "Slytherin", conmsg6);
    int fdcon6 = open("gevent", O_WRONLY);
    if (fdcon6 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon6, conmsg6, MAX_BUF);
    close(fdcon6);
    sleep(1);
    check_domain("Slytherin"); // Checks if Slytherin Domain exists.
    check_rd_wr_pipe("Slytherin", "Draco_RD", "Draco_WR");

    // Client named Blaise wants to connect to domain Slytherin.
    char conmsg7[MAX_BUF] = {0};
    connect("Blaise", "Slytherin", conmsg7);
    int fdcon7 = open("gevent", O_WRONLY);
    if (fdcon7 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon7, conmsg7, MAX_BUF);
    close(fdcon7);
    sleep(1);
    check_rd_wr_pipe("Slytherin", "Blaise_RD", "Blaise_WR");

    return 0;

}
