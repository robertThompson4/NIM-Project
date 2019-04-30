// playNim.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playTicTacToe() which is defined below


#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>
#include "nim.h"
using namespace std;

void initializeBoard( vector<int> &board)
{
	int temp = 5;
	while (temp > 2 && temp < 10) {
		cout << "How many rows would you like? (3-9): ";
		cin >> temp;
	}
	board.push_back(temp);
	for (int i = 1; i <= temp; i++)
	{
		while (temp > 0 && temp < 21) {
			cout << "How many rocks would you like in row " << i << "? (1-20): ";
			cin >> temp;
		}
		if (temp <= 9)
		{
			board.push_back(0);
			board.push_back(temp);
		}
		else
		{
			board.push_back(temp / 10);
			board.push_back(temp % 10);
		}

	}
}

void updateBoard( vector<int> board, int move)
{
	
}

void displayBoard( vector<int> board)
{
	int numStones;
	cout << endl;
	for (int pile = 1; pile < board[0]; pile++)
	{
		cout << "Pile " << pile << ": ";
		numStones = board[(pile * 2) - 1] * 10 + board[pile * 2];
		for (int j = 1; j < numStones; j++)
		{
			cout << "* ";
		}
		cout << endl;
	}
	cout << endl;
}

bool check4Win(vector<int> board)
{
	
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
	
	move_str[0] = pileNum - '0';
	move_str[1] = (board[(pileNum * 2) - 1]) - '0';
	move_str[2] = (board[pileNum * 2], str) - '0';
	move_str[3] = '\n';
	return string(move_str);
}

int playNim(SOCKET s, std::string serverName, std::string remoteIP, std::string remotePort, int localPlayer)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	vector<char> board;
	int opponent;
	int move;
	bool myMove;
	char movestr[2] = "";

	if (localPlayer == PLAYER_X) {
		std::cout << "Playing as X" << std::endl;
		opponent = PLAYER_O;
		myMove = true;
	} else {
		std::cout << "Playing as O" << std::endl;
		opponent = PLAYER_X;
		myMove = false;
	}

	initializeBoard(board);
	displayBoard(board);

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board
			move = getLocalUserMove(board, localPlayer);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(board,move,localPlayer);
			displayBoard(board);

			// Send move to opponent
/****			
	Task 1: "move" is an integer that was assigned a value (from 1 to 9) in the previous code segment.
	         Add code here to convert "move" to a null-terminated C-string and send it to your opponent at remoteIP using remotePort.
****/

			
			_itoa_s(move, movestr, 2, 10);
			UDP_send(s, movestr, MAX_SEND_BUFFER, (char*)remoteIP.c_str(), (char*)remotePort.c_str());


		} else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
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
				UDP_recv(s, movestr, 2, hostIP, hostPort);
				updateBoard(board, atoi(movestr), opponent);
				displayBoard(board);

			} else {
				winner = ABORT;
			}
		}
		myMove = !myMove;	// Switch whose move it is

		if (winner == ABORT) {
			std::cout << timestamp() << " - No response from opponent.  Aborting the game..." << std::endl;
		} else {
			winner = check4Win(board);
		}
		
		if (winner == localPlayer)
			std::cout << "You WIN!" << std::endl;
		else if (winner == TIE)
			std::cout << "It's a tie." << std::endl;
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}