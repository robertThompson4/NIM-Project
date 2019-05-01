// server_main.cpp
//   This function serves as the "main" function for the server-side
#include <iostream>
#include <string>
#include <WinSock2.h>
#include "nim.h"


int serverMain(int argc, char *argv[], std::string playerName)
{	//
	SOCKET s;
	char buffer[MAX_RECV_BUFFER];
	char host[v4AddressSize];
	char port[portNumberSize];
	char responseStr[MAX_SEND_BUFFER];

	s = passivesock(NIM_UDPPORT, "udp");

	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buffer, MAX_RECV_BUFFER, host, port);
	if (debug) {
		std::cout << timestamp() << " - Received: " << buffer << " from " << host << ":" << port << std::endl;
	}

	/*if (strcmp(buffer, "Who?"))
	{
		char response[MAX_SEND_BUFFER] = "Your favorite Server";
		UDP_send(s, response, MAX_SEND_BUFFER, host, port);
	}*/



	bool finished = false;
	while (!finished) {
		if (strcmp(buffer, NIM_QUERY) == 0) {
			// Respond to name query
			strcpy_s(responseStr, NIM_NAME);
			strcat_s(responseStr, playerName.c_str());
			if (debug) {
				std::cout << timestamp() << " - Sending: " << responseStr << " to " << host << ":" << port << std::endl;
			}
			UDP_send(s, responseStr, strlen(responseStr) + 1, host, port);
			//
		}
		else if (strncmp(buffer, NIM_CHALLENGE, strlen(NIM_CHALLENGE)) == 0) {
			// Received a challenge  
			char *startOfName = strstr(buffer, NIM_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName + strlen(NIM_CHALLENGE) << std::endl;
			}

			if (startOfName != NULL) {
				bool checkWantToPlay = false;
				std::cout << "Do you want to Play" << std::endl;
				std::string answer = "";
				std::cin >> answer;

				for (std::string::size_type i = 0; i < answer.length(); ++i)
					 toupper(answer[i]);

				if (answer != "NO")
				{
					char xtraBuffer[MAX_RECV_BUFFER];
					
					UDP_send(s, NIM_SERVER_YES, strlen(NIM_SERVER_YES) + 1, host, port);
					int status = wait(s, 20, 0);
					UDP_recv(s, xtraBuffer, MAX_RECV_BUFFER, host, port);
					
					if (status > 0) {
						if (strcmp(xtraBuffer, NIM_SERVER_GREAT) == 0)
						{
							// Play the game.  You are the 'O' player
							int winner = playNim(s, playerName, host, port, 1);
							finished = true;
						}
					}
				}
				else
				{
					char xtraBuffer[MAX_RECV_BUFFER]="Sorry Try again soon!";
					UDP_send(s, xtraBuffer, strlen(NIM_SERVER_YES) + 1, host, port);
				}
			}
		}

		if (!finished) {
			char previousBuffer[MAX_RECV_BUFFER];	strcpy_s(previousBuffer, buffer);
			char previousHost[v4AddressSize];				strcpy_s(previousHost, host);
			char previousPort[portNumberSize];				strcpy_s(previousPort, port);

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			int status = wait(s, 1, 0);	// We'll wait a second to see if we receive another datagram
			while (!newDatagram && status > 0) {
				len = UDP_recv(s, buffer, MAX_RECV_BUFFER, host, port);
				if (debug) {
					std::cout << timestamp() << " - Received: " << buffer << " from " << host << ":" << port << std::endl;
				}
				if (strcmp(buffer, previousBuffer) == 0 &&		// If this datagram is identical to previous one
					strcmp(host, previousHost) == 0 &&		//  and it came from a previously known host
					strcmp(port, previousPort) == 0) {		//  using the same port number, then ignore it.
					status = wait(s, 1, 0);			// Wait another second (still more copies?)
				}
				else {
					newDatagram = true;		// if not identical to previous one, keep it!
				}
			}

			// If we waited one (or more seconds) and received no new datagrams, wait for one now.
			if (!newDatagram) {
				len = UDP_recv(s, buffer, MAX_RECV_BUFFER, host, port);
				if (debug) {
					std::cout << timestamp() << " - Received: " << buffer << " from " << host << ":" << port << std::endl;
				}
			}
		}
	}
	closesocket(s);

	return 0;
}
