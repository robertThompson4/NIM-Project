// nim.h    Header file for Nim project
#pragma once
#include <WinSock2.h>
#include <string>
#define debug true

#define NIM_QUERY	"Who?"
#define NIM_NAME	"Name="
#define NIM_CHALLENGE	"Player="
#define NIM_SERVER_YES  "YES"
#define NIM_SERVER_GREAT "GREAT!"

static char NIM_UDPPORT[] = "29333";

const int v4AddressSize = 16;
const int portNumberSize = 10;
const int WAIT_TIME = 120;
const int MAX_SEND_BUFFER = 81;
const int MAX_RECV_BUFFER = 2048;
const int MAX_SERVERS = 100;
const int MAX_NAME = 100;
const int noWinner = 0;

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

int playNim(SOCKET, std::string, std::string, std::string, int);
SOCKET connectsock(const char*, const char*, const char*);
SOCKET passivesock(const char*, const char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, const char*, int, const char*, const char*);
int wait(SOCKET, int, int);
char* timestamp();
int getServers(SOCKET, const char*, const char*, ServerStruct[]);
int serverMain(int, char *argv[], std::string);
int clientMain(int, char *argv[], std::string);
int getIPAddressInfo(char*, char*);