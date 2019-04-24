// TicTacToe.h
//
// This header file establishes some constants that should be used in the TicTacToe project for COMP 311
#include <WinSock2.h>
#include <string>

#define debug true
static char TicTacToe_UDPPORT[] = "28900";			// Port number used by TicTacToe servers

#define TicTacToe_QUERY  "Who?"
#define TicTacToe_NAME	 "Name="
#define TicTacToe_CHALLENGE "Player="

const int v4AddressSize = 16;
const int portNumberSize = 10;
const int MAX_INTERFACES = 20;
const int WAIT_TIME = 60;
const int MAX_RECV_BUFFER = 2048;
const int MAX_SEND_BUFFER = 2048;
const int MAX_SERVERS = 100;
const int PLAYER_X = 1;
const int PLAYER_O = 2;
const int ABORT = -1;
const int noWinner = 0;
const int winnerIsX = 1;
const int winnerIsO = 2;
const int TIE = 3;

static char board[10];

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

SOCKET connectsock(const char*, const char*, const char*);
SOCKET passivesock(const char*, const char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, const char*, int, const char*, const char*);
int wait(SOCKET, int, int);
char* timestamp();
int getServers(SOCKET, const char*, const char*, ServerStruct[]);
int playTicTacToe(SOCKET, std::string, std::string, std::string, int);
int serverMain(int, char *argv[], std::string);
int clientMain(int, char *argv[], std::string);
int getIPAddressInfo(char*, char*);
