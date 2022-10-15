//Nathan Bunge, Tom Arad, Jared Hartwig, Jake Bortles
//Lab Section 3 (exept Tom in lab 7)
//4/31/2020
//Cpts 122 Spring 2020
//Professor: Nadra
//PA 9: Connect 4
//Description: Classic 4 in a row game. Includes graphics using SFML. 
//Is able to connect to another game that is running if both comptuers are using the same internet
//WARNING: will not run on linux! (sorry)

#include "game.h"

class connectGame :public game
{
private:
	//board array
	char board[6][7] = {};

	//circles that will go on the vitual board
	CircleShape circles[6][7];

	//other window SFML variable that will be set later
	Texture texture;
	CircleShape arrow;
	RectangleShape window_board = RectangleShape(Vector2f(795, 675));
	RectangleShape border = RectangleShape(Vector2f(815, 695));
	Texture texture3;
	Sprite cover;
	Text person;
	Text turn_text;
	Text winner;
	CircleShape circle1;
	CircleShape circle2;

	//position tracker (for moving left and right)
	int position;

	//local and romote turn functions
	void local_turn();
	void remote_turn();

public:
	//constructor
	connectGame();


	//runs 1 turn
	void turn();

	//creates things for board
	void draw_board();

	//creates a window that displays the board
	void display_current_board();

	//set values for arrow
	void init_arrow();

	//tests that check for horizontal, vertical, and diagonal combinations
	bool testHorizontal(char player);
	bool testVertical(char player);
	bool testDiagonal(char player);

	//tests if board is full
	bool testTie();


	//places chip, returns false if column is full
	bool placeChip(int column, char player);

	//for debugging
	void display_board();

	//runs the game
	virtual void play();

	//set values for board
	void init_board();

};