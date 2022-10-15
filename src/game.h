//Nathan Bunge, Tom Arad, Jared Hartwig, Jake Bortles
//Lab Section 3 (exept Tom in lab 7)
//4/31/2020
//Cpts 122 Spring 2020
//Professor: Nadra
//PA 9: Connect 4
//Description: Classic 4 in a row game. Includes graphics using SFML. 
//Is able to connect to another game that is running if both comptuers are using the same internet
//WARNING: will not run on linux! (sorry)
#pragma once

#include <iostream>
#include <time.h>
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>

#include "NetworkConnection.h"

using namespace std;
using namespace sf;

class game
{
private:


protected:

	Font font;
	Text text;
	RenderWindow *window = nullptr;
	char player_sign;
	//gamestate, 0 is normal, 1 is p1 win, 2 is p2 win, 3 is tie
	int gameState;

	//keeps track of who's turn it currently is
	char playerturn;

	//name of p1
	string local_user;

	//name of p2
	string remote_user;


	int current_col;
	int current_row;;

	NetworkConnection net;

public:
	//constructor
	game();
	~game();
	//destructor
	//virtual ~game();

	virtual void play();
	//sets usernames
	void setUsernames();

	//says who won the game
	void declareWinner();

	//checks if someone won
	bool checkWinner(char);

	//testing for in a row, returns 1 if found, 0 if not
	bool virtual testHorizontal(char) = 0;
	bool virtual testVertical(char) = 0;
	bool virtual testDiagonal(char) = 0;
	bool virtual testTie() = 0;

	static void show_rules();


	//delay function
	void delay(int milli_seconds);
	void barrier();

};


/*
class tictacGame :public game
{
private:
	char board[3][3] = {};

public:
	tictacGame();



	bool testHorizontal(char num);
	bool testVertical(char num);
	bool testDiagonal(char num);

};


class gomokuGame : public game
{
private:
	char board[15][15] = {};

public:
	gomokuGame();


	bool testHorizontal();
	bool testVertical();
	bool testDiagonal();

};

*/
