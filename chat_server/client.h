#ifndef CLIENT_H
#define CLIENT_H

#define MAX_ID (256)
#define TERM (255)
#define MAX_BUF (2048)
#define END_ID (258)
#define START_PKT (2)
#define MAX_PNAME (262)
#define MSG_LEN1 (10)
#define MSG_LEN2 (11)

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

int check_gevent();
int check_domain(char* dom);
int check_rd_wr_pipe(char* dom, char* pipe_rd, char* pipe_wr);
void connect(char* id_name, char* domain_name, char* newmsg);
void say(char* id_name, char* newmsg);
void saycont(char* id_name, char* domain_name, char* newmsg, int num);

#endif
