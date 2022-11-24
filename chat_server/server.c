#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include "server.h"
#include <string.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// Helper function that creates the pipe name and path.
void get_pipe_name(char* id, char* dom, char* pipe_name, int type){

    const char prefix[]= "./";
    const char slash[] = "/";
    char postfix[4];

    switch(type){
        case(PREAD):
        {
            strcpy(postfix, "_RD");
            break;
        }

        case(PWRITE):
        {
            strcpy(postfix, "_WR");
            break;
        }
    }

    // Concatenates the prefix and postfix to make the pipe name and path.
    strcpy(pipe_name, prefix);
    strcat(pipe_name, dom);
    strcat(pipe_name, slash);
    strcat(pipe_name, id);
    strcat(pipe_name, postfix);

}

// Helper function that creates the relative path of the filename when 
// traversing through the directory to find the pipes to relay message.
void get_relative_path(char* filename, char* dom, char* pipe_name){

    const char prefix[]= "./";
    const char slash[] = "/";

    strcpy(pipe_name, prefix);
    strcat(pipe_name, dom);
    strcat(pipe_name, slash);
    strcat(pipe_name, filename);

}

// Executes message of clients CONNECT from gevent.
int execute_msg(char* id, char* dom, char* pipe_wr, char* pipe_rd){

    // Checking of domain already exists.
    pid_t pid;
    int is_child = FALSE;
    DIR* dirdom = opendir(dom);
    // If domain folder does not exist,
    // the new domain folder is created.
    if (!dirdom){
        int res= mkdir(dom, 0777);
        if (res == -1){
            fprintf(stderr, "Unable to create folder %s.\n", dom);
            exit(EXIT_FAILURE);
        }
    }else{
        closedir(dirdom);
    }

    // Get the _RD and _WR pipe names for the new client.
    get_pipe_name(id, dom, pipe_rd, PREAD);
    get_pipe_name(id, dom, pipe_wr, PWRITE);

    // Create the _RD and _WR pipes for client.
    int read = mkfifo(pipe_rd, 0777);
    int write = mkfifo(pipe_wr, 0777);

    // Checks if there is error in creating pipe.
    if (read <0){
        fprintf(stderr, "Error creating named pipe _RD.\n");
        perror("mkfifo() failed");
        exit(EXIT_FAILURE);
    }else if (write <0){
        fprintf(stderr, "Error creating named pipe _WR.\n");
        perror("mkfifo() failed");
        exit(EXIT_FAILURE);
    }

    // Forks a child, that is, the client handler for the new connected client.
    pid = fork();

    // returns TRUE if it is the child process, so that it breaks from the
    // global process' loop and become child handler.
    if (pid == 0){
        is_child = TRUE;
    }
    return is_child;
}

// Helper function that extracts the SAY message of clients.
// So that it can be included in the new message of RECV or RECVCONT
// that client handler will relay to other client handlers.
int parse_say (char* buffer, char* msg, int is_cont){

    int is_term= -1;
    short bidx = START_PKT;
    short midx = 0;
    short end_msg = MAX_BUF;

    // If it is a SAYCONT message, then length of messgage is 1789
    // and extract termination byte if exists.
    if (is_cont) {
        end_msg = MAX_BUF -1;
        unsigned char tbyte = buffer[MAX_BUF-1];
        if (tbyte == TERM){
            is_term = TRUE;

        }else{
            is_term = FALSE;
        }
    }
    while (buffer[bidx]!= '\0' && bidx !=end_msg){
        msg[midx] = buffer[bidx];
        midx++;
        bidx++;
    }

    return is_term;

}
// Helper function that creates new message of RECV or RECVCONT
// that client handler will relay to other client handlers.
void create_newmsg (char* id, char* msg, unsigned char* newmsg, int is_term){

    // Sets the message type if it is a RECV or RECVCONT.
   unsigned char type = 0;
    if (is_term == -1){
        type = RECEIVE;
    }else{
        type = RECVCONT;
    }

    // Inputs id and client message inside new RECV or RECVCONT message.
    newmsg[0] = type;
    newmsg[1] = 0;
    short nidx = START_PKT;
    short ididx = 0;
    short midx = 0;
    while (id[ididx] != '\0' && ididx<= MAX_ID){
        newmsg[nidx] = id[ididx];
        nidx++;
        ididx++;

    }

    nidx = END_ID;
    while (msg[midx] != '\0' && midx <= MAX_MSG){
        newmsg[nidx] = msg[midx];
        nidx++;
        midx++;

    }

    // Adds termination byte if it existed in the previous SAYCONT message.
    if (is_term == TRUE){
        unsigned char tbyte = TERM;
        newmsg[MAX_BUF-1] = tbyte; 
    }

}

// Helper function that checks if file inside directory is a valid file, that is,
// a _WR pipe that is not its own _WR pipe.
int is_validwr(char* filename, char* pipe_wr){

    int is_valid= FALSE;
    const char wr[] = "_WR";
    size_t lenfn = strlen(filename);
    size_t lenwr = strlen(wr);

    // Checks if the end of the file name is "_WR".
    if (lenfn > lenwr){
        size_t lenpwr = strlen(pipe_wr);
        /* USYD CODE CITATION ACKNOWLEDGEMENT
        * I declare that the following lines of code have been copied from the
        * website titled: "How to compare ends of strings in C?"
        * and it is not my own work.
        *
        * Original URL
        * //https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
        * Last access May, 2021
        */
        if (strncmp(pipe_wr+lenpwr-lenfn, filename, lenfn) == 0){ // If it is their own _wr pipe, it is invalid.
            return is_valid;
        }
        is_valid = strncmp(filename+lenfn-lenwr, wr, lenwr) == 0;
        /* end of copied code */
    }

    return is_valid;

}

// Sends (relays) RECV or RECVCONT message other client handlers in domain.
void send_others (unsigned char* msg, char* dom, char* pipe_wr){
    
    /* USYD CODE CITATION ACKNOWLEDGEMENT
    * I declare that the following lines of code have been copied from the
    * website titled: "C Program to List Files in Directory."
    * with only minor changes and it is not my own work.
    *
    * Original URL
    * //https://www.sanfoundry.com/c-program-list-files-directory/
    * Last access May, 2021
    */
    DIR* dir;
    struct dirent* domain;
    char filename[MAX_PNAME];
    char pipe_name[MAX_PNAME];
    dir = opendir(dom);

    // Traverse throught the whole domain folder to write to other client's _WR pipes
    // to relay the message.
    if (dir){
        while((domain= readdir(dir)) != NULL){
            memset(filename, 0,MAX_NAME* sizeof(filename[0]));
            memset(pipe_name, 0,MAX_NAME* sizeof(pipe_name[0]));
            strcpy(filename, domain->d_name);

            // Writes to only valid files, that is, _WR pipes excluding its own
            // client's _WR pipe.
            if(is_validwr(filename, pipe_wr) ){
                get_relative_path(filename, dom, pipe_name);
                int pipe_WR = open(pipe_name, O_WRONLY);
                if (pipe_WR == -1){
                    fprintf(stderr, "Unable to open pipe %s\n", pipe_name);
                }
                int wrres = write (pipe_WR, msg, MAX_BUF);
                if(wrres == -1){
                    fprintf(stderr, "Unable to write to pipe %s\n", pipe_name);
                }

                close(pipe_WR);
            }
        }

    }
    /* end of copied code */
}

// Executes the message that client handler receives from client's _WR pipe.
int execute_client_msg(char* buffer, char* dom, char* id, char* pipe_rd, char* pipe_wr){

    short type = buffer[0];
    char cid[MAX_NAME];
    char msg[MAX_MSG];
    int send_back = FALSE;
    unsigned char newmsg[MAX_BUF];
    memset(cid, 0,MAX_NAME* sizeof(id[0]));
    memset(msg, 0,MAX_MSG* sizeof(msg[0]));
    memset(newmsg, 0, MAX_BUF* sizeof(newmsg[0]));

    // Executes messages according to their type
    switch (type){
        // SAY and SAYCONT messages are transformed into RECV messages
        // and relayed to other client handlers.
        case(SAY):
        {
            parse_say(buffer, msg, FALSE);
            create_newmsg(id, msg, newmsg, -1);
            send_others(newmsg, dom, pipe_wr);
            break;
        }
        case (SAYCONT):
        {
            int is_term = parse_say(buffer, msg, TRUE);
            create_newmsg(id, msg, newmsg, is_term);
            send_others(newmsg, dom, pipe_wr);
            break;
        }

        // RECV and RECVCONT messages are relayed to its own client.
        case (RECEIVE):
        {
            send_back = TRUE;
            break;
        }
        case(RECVCONT):
        {
            send_back = TRUE;
            break;
        }

        // DISCONNECT will make client handler deletes the _WR and _RD pipe 
        // and kills itself, where global process will reap it.
        case (DISCONNECT):
        {
            unlink(pipe_rd);
            unlink(pipe_wr);
            exit(0);
            break;
        }

        // Invalid messages that does not belong to any type will make the client handler
        // send back a message to the sender that the message is invalid.
        default:
            printf("Message of invalid type.\n");
            send_back = -1;
            break;
    }

    return send_back;

}

// Main function for client handler process.
void become_client_handler(char* pipe_wr, char* pipe_rd, char* dom, char* id){

    char buffer[MAX_BUF];
    struct pollfd pipes[1];
    ssize_t pwrite = open(pipe_wr, O_RDONLY);

    if (pwrite == -1){
        fprintf(stderr, "Unable to open pipe %s\n", pipe_wr);
    }

    int send_back = FALSE;
    pipes[0].fd = pwrite;
    pipes[0].events = POLLIN;

    // Checks for incoming messages from _WR pipe by using Poll.
    while (RUNNING){
        memset(buffer, 0, MAX_BUF* sizeof(buffer[0]));
        usleep(500);

        int polres = poll(pipes, 1, -1);
        if (polres == -1){
            fprintf(stderr, "%s : Unable to poll.\n", id);
        }

        // Reads _WR pipe only if its ready.
        short rrevents = pipes[0].revents;
        if (rrevents & POLLIN){
            read(pwrite,buffer, MAX_BUF);
            send_back = execute_client_msg(buffer, dom, id, pipe_rd, pipe_wr);

        }

        // Sends RECV and RECVCONT messages to client, 
        // or invalid messages from client.
        if (send_back || send_back == -1){

            if(send_back == -1){
                memset(buffer, 0, MAX_BUF* sizeof(buffer[0]));
                strcpy(buffer, "Message of invalid type.");
            }
            ssize_t pread = open(pipe_rd, O_WRONLY);
            int wrres = write(pread,buffer, MAX_BUF);
            if(wrres == -1){
                fprintf(stderr, "Unable to write to pipe.\n");
            }

            close(pread);
            send_back = FALSE;
        }

    }

}

// Function that handles SIGCHLD signals, to reap dead child processes.
void handle_sigchld(int sig){
    /* USYD CODE CITATION ACKNOWLEDGEMENT
    * I declare that the following lines of code have been copied from the
    * website titled: "Reap zombie processes using a SIGCHLD handler."
    * and it is not my own work.
    *
    * Original URL
    * //http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler
    * .html#:~:text=The%20process%20of%20eliminating%20zombie,or%20SIGCHLD%20to%20reap%20asynchronously

    * Last access May, 2021
    */
  int saved_errno = errno;
  waitpid((pid_t)(-1), 0, WNOHANG);
  errno = saved_errno;
  /* end of copied code */
}

// Parses the connect message into domain, id, and type (which should be of type CONNECT)
// from incoming messages from gevent.
void parse_connect(short* type, char* dom, char* id, unsigned char* buffer){

    *type = (short) buffer[0];
    short bidx = START_PKT;
    short ididx = 0;
    while (buffer[bidx] != '\0' && bidx<=END_ID){
        id[ididx] = buffer[bidx];
        ididx++;
        bidx++;                
    }

    bidx = END_ID;
    short domidx = 0; 
    while (buffer[bidx] != '\0' && bidx<=MAX_BUF){
        dom[domidx] = buffer[bidx];
        domidx++;
        bidx++;
    }
}

// Main loop for global process.
void run_chat(){

    short type = -1;
    char id[MAX_NAME];
    char dom[MAX_NAME];
    char gevpipe[] = "./gevent";
    struct pollfd fds[1];
    unsigned char buffer[MAX_BUF];
    char pipe_rd[MAX_PNAME];
    char pipe_wr[MAX_PNAME];
    int is_child = FALSE;

    /* USYD CODE CITATION ACKNOWLEDGEMENT
    * I declare that the following lines of code have been copied from the
    * website titled: "Reap zombie processes using a SIGCHLD handler."
    * and it is not my own work.
    *
    * Original URL
    * //http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler
    * .html#:~:text=The%20process%20of%20eliminating%20zombie,or%20SIGCHLD%20to%20reap%20asynchronously

    * Last access May, 2021
    */
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror(0);
        exit(1);
    }
    /* end of copied code */

    // Create gevent.
    int res = mkfifo(gevpipe, 0777);
    if (res == -1){
        fprintf(stderr, "Unable to create gevent.\n");
        exit(EXIT_FAILURE);
    }

    ssize_t gevent = open(gevpipe, O_RDONLY);
    fds[0].fd = gevent;
    fds[0].events = POLLIN;

    // Loops to listen Poll events, and breaks if it is a child process
    // to become child handler.
    while (RUNNING && !is_child){

        memset(id, 0, MAX_NAME* sizeof(id[0]));
        memset(dom, 0, MAX_NAME* sizeof(dom[0]));
        memset(buffer, 0, MAX_BUF* sizeof(buffer[0]));

        usleep(500);
        poll(fds, 1, -1);

        // Reads from gevent if ready.
        short revents = fds[0].revents;
        if (revents & POLLIN){
            read(gevent,buffer, sizeof(buffer));
            parse_connect(&type, dom, id, buffer);

            // Creates domain (if necessary) and _WR _RD pipes.
            if (id[0] != 0 && dom[0] != 0 && type == CONNECT){
                is_child = execute_msg(id, dom, pipe_wr, pipe_rd);
            }

        }

    }

    // Transforms into child handler if it is a child process.
    if(is_child){
        become_client_handler(pipe_wr, pipe_rd, dom, id);
    }else{
        unlink(gevpipe);

    }

}

int main(int argc, char** argv) {
    run_chat();
    return 0;
}
