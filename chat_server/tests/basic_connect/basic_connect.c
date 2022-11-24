#include "../../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

// Checks when one client connects to domain.
int main(){

    char conmsg[MAX_BUF] = {0};
    check_gevent(); // Checks if gevent exists.

    // Connect() creates the binary message.
    connect("Ron", "Gryffindor", conmsg);
    int fdcon = open("gevent", O_WRONLY);
    if (fdcon == -1){
        printf("Failed opening gevent.\n");
    }
    
    // Writing to gevent.
    write(fdcon,conmsg, MAX_BUF);
    close(fdcon);
    usleep(1000);
    check_domain("Gryffindor"); // Checks if the domain exists.
    check_rd_wr_pipe("Gryffindor", "Ron_RD", "Ron_WR"); // Check if the _RD and _WR pipe exists.

    return 0;

}