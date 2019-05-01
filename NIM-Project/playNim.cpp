// playNim.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playTicTacToe() which is defined below


#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>
#include "nim.h"
#pragma comment (lib, "ws2_32.lib")
using namespace std;

void initializeBoard( vector<int>& board)
{
	int temp = 0;
	while (temp < 2 || temp > 10) {
		cout << "How many rows would you like? (3-9): ";
		cin >> temp;
	}
	board.push_back(temp);
	for (int i = 1; i <= temp; i++)
	{
		int tempTwo=0;
		while (tempTwo <= 0 || tempTwo > 21) {
			cout << "How many rocks would you like in row " << i << "? (1-20): ";
			cin >> tempTwo;
		}
		if (tempTwo <= 9)
		{
			board.push_back(0);
			board.push_back(tempTwo);
		}
		else
		{
			board.push_back(tempTwo / 10);
			board.push_back(tempTwo % 10);
		}

	}
}

void updateBoard( vector<int>& board, string move)
{
	int tens = ((move[0] - '0') * 2) - 1;
	int ones = (move[0] - '0') * 2;
	board[tens] = board[tens] - (move[1] - '0');
	board[ones] = board[ones] - (move[2] - '0');
}

void displayBoard( vector<int> board)
{
	int numStones;
	cout << endl;
	for (int pile = 1; pile < board[0]+1; pile++)
	{
		cout << "Pile " << pile << ": ";
		numStones = board[(pile * 2) - 1] * 10 + board[pile * 2];
		for (int j = 1; j < numStones+1; j++)
		{
			cout << "* ";
		}
		cout << endl;
	}
	cout << endl;
}

bool check4Win(vector<int> board)
{
	for (int i = 1; i < board.size(); i++)
	{
		if (board[i] != 0)
			return false;
	}
	return true;
}

string getLocalUserMove(vector<int> &board, int player)
{
	int pileNum;
	int numRocks;
	char move_str[4];
	char str;
	bool valid = true;

	do {
		valid = true;
		cout << "Which pile would you like to remove rocks from? ";
		cin >> pileNum;
		if (pileNum < 1 || pileNum > board.size() / 2)
		{
			cout << "The pile selected is not a valid pile." << endl;
			valid = false;
		}
	} while (!valid);
	

	do {
		valid = true;
		cout << "How many rocks would you like to remove from pile " << pileNum << "? ";
		cin >> numRocks;
		int rocksAvailable = board[(pileNum * 2) - 1] * 10 + board[pileNum * 2];
		if (numRocks < 1 || numRocks > rocksAvailable)
		{
			cout << "The number of rocks selected is invalid." << endl;
			valid = false;
		}
	} while (!valid);
	
	move_str[0] = pileNum + '0';
	move_str[1] = (numRocks/10) + '0';
	move_str[2] = (numRocks%10) + '0';
	move_str[3] = '\0';
	return string(move_str);
}

int playNim(SOCKET s, string serverName, string remoteIP, string remotePort, int localPlayer)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	int opponent;
	bool myMove;
	bool win;
	char movestr[MAX_SEND_BUFFER];
	string move;
	vector<int> board;
	char boardSend[MAX_SEND_BUFFER];
	initializeBoard(board);
	displayBoard(board);
	for (int i = 0; i < board.size(); i++)
	{
		boardSend[i] = (board[i] + '0');
	}
	boardSend[board.size()] = '\0';
	UDP_send(s,boardSend,board.size()+1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
	if (localPlayer == 1) {
		myMove = false;
	} else {
		myMove = true;
	}


	bool checkWin = true;
	while (checkWin) {
		if (myMove) {
			string action;
			string comment = "";
			bool validMove = false;
			// Get my move & display board
			while (!validMove) {
				cout << "What would you like to do? \n\"M\": Make a move\n\"C\": Send a comment to your opponent\n\"F\": Forfeit the game \n Answer: " << std::endl;
				cin >> action;
				cout << endl;
				if (action == "M" || action == "F")
				{
					validMove = true;
				}
				else if (action == "C")
				{
					cout << "What would you like to say to your opponent? ";
					cin.ignore();
					getline(cin, comment);
					cout << endl;
					comment = "C" + comment;
					char char_array[100];
					strcpy_s(char_array, comment.c_str());
					UDP_send(s, char_array, MAX_SEND_BUFFER, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
				}
				else if (action != "F" && action != "C" && action != "M")
				{
					cout << "Invalid action selected." << endl << endl;
					
				}
			}
			if (action != "F") {
				move = getLocalUserMove(board, localPlayer);
				cout << "Board after your move:" << endl;
				updateBoard(board, move);

				
				displayBoard(board);
			}
			else
			{
				cout << "You have forfeit the game. " << endl;
				winner = 2;
				checkWin = false;
				move[0] = 'F';
				move[1] = '\0';
			}

			// Send move to opponent

			//convert move string to char array

			strcpy_s(movestr, move.c_str());
			UDP_send(s, movestr, MAX_SEND_BUFFER, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
			


		} else {
			cout << "Waiting for your opponent's move..." << endl << endl;
			//Get opponent's move & display resulting board
			int status = wait(s,WAIT_TIME,0);
			if (status > 0) {
/****			
Task 2: (i) Insert code inside this IF statement that will accept a null-terminated C-string from your
		opponent that represents their move.  Convert that string to an integer and then
		(ii) call a function that will update the game board (see above) using your opponent's move, and
		(iii) call a function that will display the updated board on your screen.
****/
				char hostIP[16];
				char hostPort[6];
				movestr[0] = 'C';
				while (movestr[0] == 'C')
				{
					UDP_recv(s, movestr, 4, hostIP, hostPort);
					if (movestr[0] == 'C')
					{
						cout << string(movestr).substr(1);
					}
					else if (movestr[0] == 'F')
					{
						cout << "Your opponent has forfeit the game. You win! " << endl;
						checkWin = false;
					}
					else if (movestr[0] - '0' <= board[0] && movestr[0] - '0' > 0 && movestr[1] - '0' <= board[((movestr[0] - '0') * 2) - 1] && movestr[2] - '0' <= board[(movestr[0] - '0') * 2])
					{
						updateBoard(board, movestr);
						
						
						displayBoard(board);
					}
					else
					{
						cout << "Invalid move by opponent. You win!!!" << endl;
						winner = 1;
					}
				}

			} else {
				winner = -1;
			}
		}
		myMove = !myMove;	// Switch whose move it is

		if (winner == -1) {
			cout << timestamp() << " - No response from opponent.  Aborting the game..." << endl;
		} else {
			win = check4Win(board);
		}
		
		if (win && !myMove) {
			cout << "You WIN!" << endl;
			checkWin = false;
		}
		else if (win && myMove) {
			cout << "I'm sorry.  You lost" << endl;
			checkWin = false;
		}
		
	}
	return winner;
}
