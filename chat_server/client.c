#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "client.h"

// This file is for testing purposes only.

// Creates messages of client when they wants to SAYCONT in testing.
void saycont(char* id_name, char* domain_name, char* newmsg, int num){

    // Testing client will say "Hello I'm <id>", 
    // "Nice to meet you all", "I'm from <domain>".
    unsigned char type = SAYCONT;
    newmsg[0] = type;
    newmsg[1] =  0;

    if (num == 0){
        strcpy((char*) newmsg+START_PKT, "Hello I'm ");
        strcat((char*) newmsg+MSG_LEN1, id_name);
    }else if (num == 1){
        strcpy((char*) newmsg+START_PKT, "Nice to meet you all");
    }else {
        strcpy((char*) newmsg+START_PKT, "I'm from ");
        strcat((char*) newmsg+MSG_LEN2, domain_name);
        newmsg[MAX_BUF-1] = (char ) TERM;
    }

}

// Creates messages of client when they wants to SAY in testing.
void say(char* id_name, char* newmsg){

    // Testing client will say "Hello I'm <id>".
    unsigned char type = SAY;
    newmsg[0] = type;
    newmsg[1] =  0;
    strcpy(newmsg+START_PKT, "Hello I'm ");
    strcat(newmsg+MSG_LEN1, id_name);
}

// Creates messages of client when they wants to CONNECT.
void connect(char* id_name, char* domain_name, char* newmsg){
   unsigned char type = CONNECT;

    newmsg[0] = type;
    newmsg[1] =  0;
    short nidx = START_PKT;
    short ididx = 0;
    short didx = 0;
    while (id_name[ididx] != '\0' && ididx<= MAX_ID){
        newmsg[nidx] = id_name[ididx];
        nidx++;
        ididx++;

    }

    nidx = END_ID;
    while (domain_name[didx] != '\0' && didx <= (MAX_BUF - MAX_ID - START_PKT)){
        newmsg[nidx] = domain_name[didx];
        nidx++;
        didx++;

    }

}

// Checks if gevent pipe exists.
int check_gevent(){
    DIR* dir;
    struct dirent* folder;
    char filename[MAX_PNAME];
    char pipe_name[] = "gevent";
    dir = opendir(".");

    if (dir){
        while((folder= readdir(dir)) != NULL){
            memset(filename, 0,MAX_PNAME* sizeof(filename[0]));
            strcpy(filename, folder->d_name);
            if (strcmp(filename, pipe_name) == 0){
                printf("Pipe gevent is open\n");
                return TRUE;
            }
        }

    }
    printf("Pipe gevent is missing\n");
    return FALSE;
}

// Checks if a given domain exists.
int check_domain(char* dom){
    DIR* dir;
    dir = opendir(dom);

    if (dir){
        closedir(dir);
        printf("Domain %s exists\n", dom);
        return TRUE;
    }
    printf("Domain %s does not exists\n", dom);
    return FALSE;
}

// Checks if _RD and _WR pipes of given id in domain exists.
int check_rd_wr_pipe(char* dom, char* pipe_rd, char* pipe_wr){
    int has_pread = FALSE;
    int has_pwrite = FALSE;
    DIR* dir;
    struct dirent* folder;
    char filename[MAX_PNAME];
    dir = opendir(dom);

    // Open domain folder and search pipes.
    if (dir){
        while((folder= readdir(dir)) != NULL){
            memset(filename, 0,MAX_PNAME* sizeof(filename[0]));
            strcpy(filename, folder->d_name);
            if (strcmp(filename, pipe_rd) == 0){
                printf("Pipe %s is open\n", pipe_rd);
                has_pread = TRUE;
            }else if (strcmp(filename, pipe_wr)== 0){
                printf("Pipe %s is open\n", pipe_wr);
                has_pwrite = TRUE;
            }
        }

    }else{
        printf("failed opening domain\n");
    }
    
    return (has_pread && has_pwrite);

}
