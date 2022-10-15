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
#include "connectGame.h"

int main()
{
	
	/* NetworkConnection con;
	con.init(); */
	string choice;

	//if this had move games implemented, here would be a switch statement where the player would choose a game
	//since this project only contains connect 4, it will go strait into the connect 4 game

	//create the connect4 game
	game* connect4;
	connect4 = new connectGame();

	//dispaly show rules window
	game::show_rules();

	while(true) // keeps running the game until says he wants to stop
	{
		//play game!
		connect4->play();

		//classic do-while loop
		do
		{
			//ask user if want to play again
			cout << "Do You Wish To Play Another Game (yes/no): ";

			//get their answer
			getline(cin, choice);

			//set their answer to uppercase so can accept low and uppercase
			for_each(choice.begin(), choice.end(), [](char& c)
				{
					c = ::toupper(c);
				});
			//if invalid choice, ask again
		} while (choice != "YES" && choice != "NO");

		//if choice is no, then stop
		if (choice == "NO")
		{
			break;
		}

		//if yes, then wait for the other player
		cout << "Waiting for the other player to accept..." << endl;

		//get the other player state
		connect4->barrier();
	}

	//deleat the game from existance
	delete connect4;

	//stop!
	return 0;
}