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

/*************************************************************
* Function:game
* Description:constructor for the game class. 
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
game::game()
{
	srand((unsigned)time(NULL));

	//initializes the host after deciding if its the server or the client
	net.init();

	//gets the usernames of the players
	setUsernames();

}

/*************************************************************
* Function:~game
* Description:destructor for the game class. amke sure deleated stuff
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
game::~game()
{
	//D E S T R U C T O R
	//(lots of destruction)
	if (window != nullptr)
	{
		delete window;
	}
	//can also display the winner here if you want
	//declareWinner();
}


/*************************************************************
* Function:play
* Description:sets the values in the game to zero. asts like a constructor
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void game::play()
{
	int i = 0;
	UserData data;
	//set gamestate to normal
	gameState = 0;

	font.loadFromFile("coolvetica_rg.ttf");
	//set usernames?

	//randomly set the first turn
	if (net.is_server())
	{
		i = rand() % 2 + 1;
		data.col = 3 - i;
		net.send(data);
	}
	else
	{
		data = net.recieve();
		i = data.col;
	}

	playerturn = (i == 1) ? '1' : '2';


	if (window != nullptr)
	{
		// Winner window from previous game
		delete window;
	}
	window = new RenderWindow(VideoMode(1000, 1000), "Connect 4 C++");


	current_col = 0;
	current_row = 0;
}

/*************************************************************
* Function:setUsername
* Description:gets the usernames from consol input
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void game::setUsernames()
{
	
	cout << "Enter Player Name: ";
	// cin >> username1;
	getline(cin, local_user); // reads spaces
	UserData data;
	data.user_name = local_user;
	net.send(data);
	cout << "Waiting for remote player to enter name..." << endl;
	data = net.recieve();
	remote_user = data.user_name;
	cout << "Remote player name: " << remote_user << endl;
}

/*************************************************************
* Function:declareWinner
* Description:dispalys a window that declares the winner according to gamestate.
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void game::declareWinner()
{
	//create font
	text = Text("", font);

	//switch statement for gamestate
	switch (gameState)
	{
	case 0://state 0, no winner
		cout << "Error, there is no winner\n";
		break;
	case 1://gamestea 1 , player 1 wins
		if (player_sign == '1')
		{
			cout << "Player 1 wins!\n"; //debugging

			//set the local user, player 1 to win
			text.setString(local_user + "\n" + "Wins!");

			//set traits for colors
			text.setOutlineColor(Color::Blue);
			text.setOutlineThickness(4.f);
		}

		//else the other player would win. 
		//This code was transfered from a local, 1 application game to a 2 application game that can connect from 2 different computers
		//that is why there is added checks for local vs remote players, instead of jsut player 1 and 2
		else
		{
			cout << "Player 1 wins!\n";//debugging

			//set the romote player to win
			text.setString(remote_user + "\n" + "Wins!");

			//set traits for colors
			text.setOutlineColor(Color::Blue);
			text.setOutlineThickness(4.f);
		}
		break;
	case 2://gamestate 2, player 2 wins
		if (player_sign == '2')
		{
			//cout to consol for dubug
			cout << "Player 2 wins!\n";

			//set the local user to win
			text.setString(local_user + "\n" + "Wins!");

			//set colors
			text.setOutlineColor(Color::Blue);
			text.setOutlineThickness(4.f);
		}
		else
		{
			//cout to colsol for debugging
			cout << "Player 2 wins!\n";

			//set the remote player to win
			text.setString(remote_user + "\n" + "Wins!");

			//set traits for colors
			text.setOutlineColor(Color::Blue);
			text.setOutlineThickness(4.f);
		}
		break;
	case 3://gaemstate 3, tie

		//cout for debugging
		cout << "It is a tie...\n";

		//set to tie
		text.setString("Tie Game");

		//set colors
		text.setOutlineColor(Color::Blue);
		text.setOutlineThickness(4.f);
		break;
	

	}

	//deleat the window from existance
	delete window;

	//create a new one :D
	window = new sf::RenderWindow(sf::VideoMode(1024, 1024), "Winner");

	//set max frames to avoid explotions
	window->setFramerateLimit(12);

	//set text charateristics
	text.setCharacterSize(150);
	text.setFillColor(sf::Color::Yellow);
	text.setPosition(175, 350);

	//import trophy image
	Texture texture;
	if (!texture.loadFromFile("Trophy.png"))
	{
		cout << "Could not load Trophy.png" << endl;
	}

	//import confettie image
	Texture texture2;
	if (!texture2.loadFromFile("Confetti3.png"))
	{
		cout << "Could not load confetti3.png" << endl;
	}

	//set trophy to a sprite so can display 
	Sprite trophy;
	trophy.setTexture(texture);
	trophy.setPosition(250, 250);

	//set confetti to a sprite so can dispaly
	Sprite confetti;
	confetti.setTexture(texture2);
	confetti.setPosition(0, 0);
	

	//draw everything
	window->draw(trophy);
	window->draw(text);
	window->draw(confetti);

	//display the window
	window->display();

	//open window till closed
	while (window->isOpen()) {

		//create event
		Event event;

		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				//close window​
				window->close();
			}
		}
	}
}

/*************************************************************
* Function:checkwinner
* Description:checks to see if a play has won by calling all the check functions
* Input parameters:the char player 1 or 2
* Returns:true if there is a winner, false if not
* Preconditions:none
* Postconditions:none
*************************************************************/
bool game::checkWinner(char player)
{
	//checks each directoin if there are a number in a row
	//returns true if there is a winnner, false if not

	//variable "player" can be changed to a color if we wish to catagorize that way
	
	//test for horizontal
	if (testHorizontal(player) == true)
	{
		if (player == '1')
		{
			//set gamestate to 1
			gameState = 1;

			cout << "Congrats on the win player 1" << endl; // debug
		}
		else
		{
			//set gamestate to 2
			gameState = 2;

			cout << "Congrats on the win player 2" << endl; // debug
		}
		
		cout << "horizontal" << endl; //debug
		return true;

	}

	//test for the vertial
	if (testVertical(player) == true)
	{
		if (player == '1')
		{
			//set gamestate to 1
			gameState = 1;

			cout << "Congrats on the win player 1" << endl; // debug
		}
		else
		{
			//set gamestate to 2
			gameState = 2;

			cout << "Congrats on the win player 2" << endl; // debug
		}
		cout << "vertical" << endl; //debug
		return true;
	}

	//test for diagonal
	if (testDiagonal(player) == true)
	{
		if (player == '1')
		{
			//set gamestate to 1
			gameState = 1;
			cout << "Congrats on the win player 1" << endl; // debug
		}
		else
		{
			//set gamestate to 2
			gameState = 2;
			cout << "Congrats on the win player 2" << endl; // debug
		}
		cout << "diagonal" << endl; //debug
		return true;
	}

	//if there is a tie
	if (testTie() == true)
	{
		gameState = 3;
		cout << "tie" << endl;
		return true;
	}

	return false;
}

/*************************************************************
* Function:show rules
* Description:dispalys the rules to the user and wait for them to press spacebar to continue
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void game::show_rules()
{

	//this will contain the menu where you can select the game
	RenderWindow window(sf::VideoMode(634, 600), "Rules");

	//import font
	Font font;
	font.loadFromFile("coolvetica_rg.ttf");

	//create the title
	Text title("", font);
	title.setString("         Connect 4         ");
	title.setStyle(Text::Underlined);
	title.setCharacterSize(75);
	title.setFillColor(Color::White);


	//create/import the picture of the kids
	Texture texture;
	if (!texture.loadFromFile("kids.png"))
	{
		cout << "Could not load kids.png" << endl;
	}
	texture.setSmooth(true);

	//set the picture of kids to a spite and set the position
	Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(0, 150);

	//background color to white
	RectangleShape background(Vector2f(630, 500));
	background.setFillColor(Color::White);
	background.setPosition(0, 100);

	//display the text with the rules
	Text text("", font);
	text.setString("                         RULES\nBe the first player to get four pieces\nin a row. Four in a row can be \nhorizontal, vertical, or diagonal.\nPress left or right to move.\nPress spacebar to place chip.\n\n\n\n          Press spacebar to begin!\n"); //idk who wrote this part but it hurts me too
	text.setCharacterSize(40);
	text.setPosition(5, 100);
	text.setOutlineColor(Color::Blue);
	text.setOutlineThickness(4.f);

	//create the circles next to the titles
	CircleShape circle1;
	CircleShape circle2;

	//set radius of circles
	circle1.setRadius(30.f);
	circle2.setRadius(30.f);

	//fill colors
	circle1.setFillColor(Color::Red);
	circle2.setFillColor(Color::Yellow);

	//set positions to next to the tilte
	circle1.setPosition(90, 20);
	circle2.setPosition(500, 20);

	//keep window open till closed
	while (window.isOpen()) {

		//create event
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				//close window​
				window.close();
			}
		}

		//draw everything
		window.draw(background);
		window.draw(sprite);
		window.draw(title);
		window.draw(text);
		window.draw(circle1);//circle1
		window.draw(circle2);//circle2

		//dispaly to window
		window.display();
	}
}

/*************************************************************
* Function:delay
* Description:pauses code for set amount of milliseconds. Used if things are going toooo faaasst
* Input parameters:number of miliseconds
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void game::delay(int milli_seconds)
{
	// Storing start time 
	clock_t start_time = clock();

	// looping till required time is not achieved 
	while (clock() < start_time + milli_seconds)
		;
}


/*************************************************************
* Function:
* Description:
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/


/*

tictacGame::tictacGame()
{
}

bool tictacGame::testHorizontal(char num)
{
	return false;
}

bool tictacGame::testVertical(char num)
{
	return false;
}

bool tictacGame::testDiagonal(char num)
{
	return false;
}



gomokuGame::gomokuGame()
{
}

bool gomokuGame::testHorizontal()
{
	return false;
}

bool gomokuGame::testVertical()
{
	return false;
}

bool gomokuGame::testDiagonal()
{
	return false;
}


*/