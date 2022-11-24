#ifndef SERVER_H
#define SERVER_H

#define RUNNING (1)
#define MAX_NAME (257)
#define MAX_BUF (2048)
#define MAX_PNAME (262)
#define START_PKT (2)
#define END_ID (258)
#define PNUM (2)
#define MAX_MSG (1790)
#define MAX_ID (256)
#define START_MSG (1791)
#define TERM (255)

typedef enum{
    CONNECT,
    SAY,
    SAYCONT,
    RECEIVE,
    RECVCONT,
    PING,
    PONG,
    DISCONNECT

}protocol;

typedef enum{
    FALSE,
    TRUE

} boolean;

typedef enum{
    PREAD,
    PWRITE

} info;

void get_pipe_name(char* id, char* dom, char* pipe_name, int type);
void get_relative_path(char* filename, char* dom, char* pipe_name);
int execute_msg(char* id, char* dom, char* pipe_wr, char* pipe_rd);
int parse_say (char* buffer, char* msg, int is_cont);
void create_newmsg (char* id, char* msg, unsigned char* newmsg, int is_term);
int is_validwr(char* filename, char* pipe_wr);
void send_others (unsigned char* msg, char* dom, char* pipe_wr);
int execute_client_msg(char* buffer, char* dom, char* id, char* pipe_rd, char* pipe_wr);
void become_client_handler(char* pipe_wr, char* pipe_rd, char* dom, char* id);
void handle_sigchld(int sig);
void parse_connect(short* type, char* dom, char* id, unsigned char* buffer);
void run_chat();

#endif
