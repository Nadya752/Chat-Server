#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when client which has a very long name wants to connect.
int main(){

    // Client named Ro<many o's>n wants to connect.
    // Check if their domain and pipe _RD _WR exists with correct name.
    char conmsg[MAX_BUF] = {0};
    check_gevent();
    connect("Roooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooon", "Gryffindor", conmsg);
    int fdcon = open("gevent", O_WRONLY);
    if (fdcon == -1){
        printf("Failed opening gevent.\n");
    }
    write(fdcon,conmsg, MAX_BUF);
    close(fdcon);
    usleep(1000);
    check_domain("Gryffindor");
    check_rd_wr_pipe("Gryffindor", "Roooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooon_RD", 
    "Roooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooon_WR");

    return 0;

}