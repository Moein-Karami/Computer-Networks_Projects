#ifndef _Defines_hpp_
#define _Defines_hpp_

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <ctime>

#define SERVER_IP "127.0.0.1"
#define COMMAND_BUF_SIZE 256
#define DATA_BUF_SIZE 4096
#define MAX_CONNECTIONS 40
#define COMMAND_PORT 8000
#define DATA_PORT 8001

#define MSG_500 "500: Error"
#define MSG_425 "425: Can't open data connection."
#define MSG_501 "501: Syntax error in parameters or arguments."
#define MSG_332 "332: Need account for login."
#define MSG_221 "221: Successful Quit."
#define MSG_226 "226: Successful Download."
#define MSG_226_2 "226: List transfer done."
#define MSG_250 "250: Successful Change."
#define MSG_550 "550: File unavailable."
#define MSG_430 "430: Invalid username or password"
#define MSG_230 "230: User logged in,procced. Logged out if appropriate."
#define MSG_503 "503: Bad sequence of commands."
#define MSG_331 "331: Username okay, need password."
#define F_MODE "-f"
#define D_MODE "-d"

#define QUITE_TYPE 0
#define OK_TYPE 1
#define BAD_CASE 0

enum ClientState{connected, waiting_for_password, logged_in};

#endif