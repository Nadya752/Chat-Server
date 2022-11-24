#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when multiple clients of same domain wants to connect.
int main(){

    // Client named Ron wants to connect to a domain named Gryffindor.
    char conmsg1[MAX_BUF] = {0};
    check_gevent(); // checks if gevent pipe exists.
    connect("Ron", "Gryffindor", conmsg1);
    int fdcon1 = open("gevent", O_WRONLY);
    if (fdcon1 == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon1, conmsg1, MAX_BUF);
    close(fdcon1);
    sleep(1);
    check_domain("Gryffindor"); // this is a new domain so we check if domain exists.
    check_rd_wr_pipe("Gryffindor", "Ron_RD", "Ron_WR"); // checks if _RD and _WR pipe exists.

    // Client named Harry wants to connect to a domain named Gryffindor.
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

    // Client named Hermione wants to connect to a domain named Gryffindor.
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

    return 0;

}