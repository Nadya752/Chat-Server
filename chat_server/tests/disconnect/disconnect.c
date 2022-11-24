#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when a client DISCONNECTs.
int main(){

    // Client named Ron connects to domain Gryffindor.
    char conmsg[MAX_BUF] = {0};
    check_gevent();
    connect("Ron", "Gryffindor", conmsg);
    int fdcon = open("gevent", O_WRONLY);
    if (fdcon == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon,conmsg, MAX_BUF);
    close(fdcon);
    usleep(1000);
    check_domain("Gryffindor");
    check_rd_wr_pipe("Gryffindor", "Ron_RD", "Ron_WR");

    // Ron decides to DISCONNECT.
    char dismsg[MAX_BUF] = {0};
    dismsg[0] = (char) DISCONNECT;
    int fddis = open("./Gryffindor/Ron_WR", O_WRONLY);
    if (fddis == -1){
        printf("Failed opening gevent.\n");
    }
    write(fddis,dismsg, MAX_BUF);
    close(fddis);
    
    sleep(1);
    // Checks if Ron's _RD and _WR pipes still exist.
    int check = check_rd_wr_pipe("Gryffindor", "Ron_RD", "Ron_WR");
    if (check == FALSE){
        printf("Pipe \"Ron_RD\" and pipe \"Ron_WR\" no longer exists.\n");
    }

    return 0;

}