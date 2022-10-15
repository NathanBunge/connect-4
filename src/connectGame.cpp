//Nathan Bunge, Tom Arad, Jared Hartwig, Jake Bortles
//Lab Section 3 (exept Tom in lab 7)
//4/31/2020
//Cpts 122 Spring 2020
//Professor: Nadra
//PA 9: Connect 4
//Description: Classic 4 in a row game. Includes graphics using SFML. 
//Is able to connect to another game that is running if both comptuers are using the same internet
//WARNING: will not run on linux! (sorry)

#include "connectGame.h"


/*************************************************************
* Function:local turn
* Description:turn for the current player on the current macheine
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::local_turn()
{
	//creates selector 
	SocketSelector selector;

	//crate data
	UserData data;

	//crate event
	Event event;

	//show board
	draw_board();

	//show the arrow
	init_arrow();

	//starting position at 3
	int position = 3;

	//set the charateristics of the name of the player
	person = Text("", font);
	person.setString(local_user);
	person.setFillColor(Color::Yellow);
	person.setCharacterSize(50);
	person.setPosition(280, 105);

	//set the hcarateristing of the winner
	winner = Text("", font);
	winner.setFillColor(Color::Black);
	winner.setCharacterSize(100);
	winner.setPosition(500, 500);




	if (window->isOpen())
	{
		while (window->pollEvent(event)) {}
	}
	while (window->isOpen())
	{
		int move = 0;
		draw_board();
		while (window->pollEvent(event))
		{
			//wait for players move
			if (selector.wait(microseconds(1))) // newer
			{
				//get data form other player
				data = net.recieve();

				//if window is closed mid-tern (rage quit)
				if (data.command == COMMAND_TERMINATE)
				{
					//dispaly the winner
					window->draw(winner);
					window->display();

					//show for a time
					delay(2000);

					//close
					window->close();

					//set to a tie
					gameState = 3;
					return;
				}
			}

			//if window is closed
			if (event.type == Event::Closed)
			{
				//changing command status to show that a player closed their window
				data.command = COMMAND_TERMINATE; //newer

				//sends data to other player to know player has closed their sceen
				net.send(data);

				//display the winner 
				window->draw(winner);
				window->display();

				//display for a short amount of time
				delay(2000);

				//clsoe
				window->close();

				//set the gamestate to a tie
				gameState = 3;
				return;
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right))
			{
				cout << "move right\n"; //for debugging
				move = 1;
			}

			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left))
			{
				cout << "move left\n"; //for dubugging
				move = -1;
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				//delay(10);
				//test if they chip can be placed here, and if it can, then place it
				if (!placeChip(position, player_sign))
				{
					continue;
				}
				//basicaly sends information to host of where they went
				data.col = position;
				net.send(data);
				//display_current_board();
				return;
				//else no chip is placed, and loop contiunes

				//can add graphics here to indicate failed placement 
			}
		}
		//lets player move left and right
		position = position + move;

		//set the arrow to follow the position
		arrow.setPosition(178 + 110 * position, 165);

		//makes sure they stay in the boundries
		if (position < 0)
			position++;
		if (position > 6)
			position--;
		//clear window and put it bakc onto the screen
		display_current_board();
	}
}

/*************************************************************
* Function:remote turn
* Description:code for the turn of the opposing player
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::remote_turn()
{
	//create selector
	sf::SocketSelector selector;
	selector.add(net.socket);

	//create even
	Event event;

	//create data
	UserData data;
	draw_board();

	//set charateristics for the name of the person (username)
	person = Text("", font);
	person.setString(remote_user);
	person.setFillColor(Color::Red);
	person.setCharacterSize(50);
	person.setPosition(280, 105);

	//set charateristic of the winner
	winner = Text("", font);
	winner.setFillColor(Color::Black);
	winner.setCharacterSize(100);
	winner.setPosition(500, 500);




	//run window till closed
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				//close window
				window->draw(winner);
				window->display();
				delay(2000);

				//changing command status to show that a player closed their window
				data.command = COMMAND_TERMINATE; //newer

				//sends data to other player to know player has closed their sceen
				net.send(data);

				window->close();

				//set the gamestate to a tie
				gameState = 3;
			}
		}

		//waits a specified amont of time to get info
		if (selector.wait(microseconds(1)))
		{
			//recives the information from the other turn
			data = net.recieve();

			//if the window was closed:
			if (data.command == COMMAND_TERMINATE)
			{
				//dispaly the window
				window->draw(winner);
				window->display();

				//display for a time
				delay(2000);

				//close
				window->close();

				//set the gamestate to a tie
				gameState = 3;
				return;
			}

			//places chip for playe 1
			if (player_sign == '1')
			{
				placeChip(data.col, '2');
			}

			//place chip for player 2
			else
			{
				placeChip(data.col, '1');
			}
			break;
		}

		//show the board
		display_current_board();
	}
}

/*************************************************************
* Function:constructor for connect game
* Description:what remains of this once powerful and important function is... well... nothing
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
connectGame::connectGame()
{
	//this does many things (not really)
}


/*************************************************************
* Function:turn
* Description:runs a turn of a player. player can move left and right until the choose a spot to palce a chip by pressing space
* Input parameters:char player 1 or 2
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::turn()
{
	int move = 0;


	//Creates Texture
	texture.setSmooth(true);

	//set max fps - this is to avoid things going to fast, like moving left and right
	window->setFramerateLimit(15);

	//check if playe 1
	if (playerturn == '1')
	{
		//run palyer 1 turn
		local_turn();

		//check if won
		checkWinner(player_sign);
	}
	else
	{
		//run other turn
		remote_turn();
		if (player_sign == '1')
		{
			//check if 2 won
			checkWinner('2');
		}
		else
		{
			//check if 1 won
			checkWinner('1');
		}
	}


	//psotion. can move form 0 to 6




}

/*************************************************************
* Function:draw baord
* Description:this SETS all the charateristis of the board. Does NOT draw or dispaly them
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::draw_board()
{
	//create array of circle objects

	//set all the objects size, color, and position
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			//create all the circles that will be in the baord
			circles[row][col].setRadius(50.f);

			//set to white if they are in an empty spot
			if (board[row][col] == NULL)
				circles[row][col].setFillColor(Color::White);

			//set to yellow if they are in a player 1 spot
			else if (board[row][col] == player_sign)
				circles[row][col].setFillColor(Color::Yellow);

			//set to red if in enemy spot
			else
				circles[row][col].setFillColor(Color::Red);

			circles[row][col].setPosition(115 + col * 110, 260 + row * 110);
		}
	}
	//Creates the board color
	window_board.setFillColor(Color(0, 0, 255));
	window_board.setPosition(100, 250);

	//creates boarder for the board
	border.setFillColor(Color::Blue);
	border.setPosition(90, 240);

	//get textur for board
	if (!texture3.loadFromFile("board.png"))
	{
		cout << "Could not load board.png" << endl;
	}
	texture.setSmooth(true);

	cover.setTexture(texture3);
	cover.setPosition(100, 250);

	//Creates the title
	text = Text("", font);
	text.setString("           Connect 4             ");
	text.setStyle(Text::Underlined);
	text.setCharacterSize(100);
	text.setFillColor(Color::White);

	//Creates text to show turn
	turn_text = Text("", font);
	turn_text.setString("Player's Turn:  ");
	turn_text.setCharacterSize(50);
	turn_text.setPosition(0, 105);
	turn_text.setFillColor(Color::White);

	//creates circles at top to indicate the turn
	circle1.setRadius(50.f);
	circle2.setRadius(50.f);
	circle1.setFillColor(Color::Red);
	circle2.setFillColor(Color::Yellow);
	circle1.setPosition(150, 10);
	circle2.setPosition(710, 10);
	circle1.setOutlineColor(Color::Blue);
	circle2.setOutlineColor(Color::Blue);
	circle1.setOutlineThickness(0.f);
	circle2.setOutlineThickness(0.f);


}

/*************************************************************
* Function:dispaly current board
* Description:this draws and DISPLAYS everything in a window. evything for the baord that is stored in the class
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::display_current_board()
{
	//clear window so we are not drawing on top of old stuff
	window->clear();

	//draw everything
	window->draw(text);
	window->draw(turn_text);
	window->draw(person);

	//highlights the circles on top for who's turn it is. also draws the arrow for the current turn
	if (playerturn == '1')
	{
		window->draw(arrow);
		circle2.setOutlineThickness(6.f);
	}
	else
	{
		circle1.setOutlineThickness(6.f);
	}

	//draw evything else
	window->draw(border);
	window->draw(window_board);
	window->draw(cover);
	window->draw(circle1);
	window->draw(circle2);


	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			window->draw(circles[row][col]);
		}
	}
	window->display();
}

/*************************************************************
* Function:inti arry
* Description:setse the charateristtics of the arrow
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::init_arrow()
{
	//set shape, color, position, and orientation
	arrow = CircleShape(28, 3);
	arrow.setFillColor(Color::Yellow);
	arrow.setPosition(175, 165);
	arrow.rotate(60.f);
	arrow.setOutlineColor(Color::Blue);
	arrow.setOutlineThickness(5.f);
}

/*************************************************************
* Function:test horizontal
* Description:checks if there are any winning combos in the horizontal direction
* Input parameters:char palyer 1 or 2 (which one to check for)
* Returns:ture if palyer has won, false if not
* Preconditions:none
* Postconditions:none
*************************************************************/
bool connectGame::testHorizontal(char player)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (i <= 3) // checking horizontal combination to the right
			{
				if ((board[j][i] == player) && (board[j][i + 1] == player) && (board[j][i + 2] == player) && (board[j][i + 3] == player))
				{
					return true;
				}
			}
			if (i >= 3) // checking horizontal combination to the left
			{
				if ((board[j][i] == player) && (board[j][i - 1] == player) && (board[j][i - 2] == player) && (board[j][i - 3] == player))
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*************************************************************
* Function:test vertical
* Description:checks if there are any winning combos in the vertical direction
* Input parameters:char palyer 1 or 2 (which one to check for)
* Returns:ture if palyer has won, false if not
* Preconditions:none
* Postconditions:none
*************************************************************/
bool connectGame::testVertical(char player)
{
	if (current_row <= 2) // checking vertical combination downwards
	{
		if ((board[current_row][current_col] == player) && (board[current_row + 1][current_col] == player) && (board[current_row + 2][current_col] == player) && (board[current_row + 3][current_col] == player))
		{
			return true;
		}
	}
	return false;
}

/*************************************************************
* Function:test diagonal
* Description:checks if there are any winning combos in the diagonal direction
* Input parameters:char palyer 1 or 2 (which one to check for)
* Returns:ture if palyer has won, false if not
* Preconditions:none
* Postconditions:none
*************************************************************/
bool connectGame::testDiagonal(char player)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (j <= 2 && i >= 3) // checking diagonal combination down left
			{
				if ((board[j][i] == player) && (board[j + 1][i - 1] == player) && (board[j + 2][i - 2] == player) && (board[j + 3][i - 3] == player))
				{
					return true;
				}

			}
			if (j <= 2 && i <= 3) // checking diagonal combination down right
			{
				if ((board[j][i] == player) && (board[j + 1][i + 1] == player) && (board[j + 2][i + 2] == player) && (board[j + 3][i + 3] == player))
				{
					return true;
				}
			}
			if (j >= 3 && i <= 3) // checking diagonal combination up right
			{
				if ((board[j][i] == player) && (board[j - 1][i + 1] == player) && (board[j - 2][i + 2] == player) && (board[j - 3][i + 3] == player))
				{
					return true;
				}
			}
			if (j >= 3 && i >= 3) // checking diagonal combination up left
			{
				if ((board[j][i] == player) && (board[j - 1][i - 1] == player) && (board[j - 2][i - 2] == player) && (board[j - 3][i - 3] == player))
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*************************************************************
* Function:testTie
* Description:checks if the board is full (if so and no winner, means a tie)
* Input parameters:none
* Returns:ture if board full, false if not
* Preconditions:none
* Postconditions:none
*************************************************************/
bool connectGame::testTie()
{
	int count = 0;

	//check through every spot in the top row
	for (int i = 0; i < 7; i++)
	{
		if (board[0][i] != '\0')
		{
			count++;
		}
	}
	//if they are all full, return true
	if (count == 7)
	{
		return true;
	}
	return false;
}


/*************************************************************
* Function:place chip
* Description:code to place a chip, aka set a spot on the board to a 1 or 2
* Input parameters:the column place, and the char player 1 or 2
* Returns:will return false if the placement was unsucessful, ture if success
* Preconditions:none
* Postconditions:none
*************************************************************/
bool connectGame::placeChip(int column, char player)
{

	//traverse to the top of the column
	int i = 0;

	if (board[i][column] != '\0') // if i = 0 after the loop it means the column is full therefore disc cannot be places here, leaving function and returning false
	{
		cout << "Column is already filled, please choose a different place\n";
		return false;
	}

	while (board[i][column] == '\0') // updating current row "i" to the amount of rows that are empty in that column
	{
		if (i == 5) // if we are on the last row do not want to update i, so leaving loop before hitting i++
		{
			break;
		}
		if (board[i + 1][column] != '\0') // checking if the next row is empty or not, if it its not leaving the loop
		{
			break;
		}
		i++;
	}

	board[i][column] = player; // passed all checks placing the matching player sign in the lowest available row in given column
	current_row = i; // updating private member current row
	current_col = column; // updating private member current col
	return true; // returning true
}

/*************************************************************
* Function:dispaly board
* Description:displays the board to the consol. for debugging
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::display_board()
{
	//for every spot in the board:
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			//print value seperated by -
			cout << board[i][j] << " - ";
		}
		cout << endl;
	}
}

/*************************************************************
* Function:paly
* Description:basicaly runs the game of connect 4
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::play()
{

	//calls game play, which sets basic variables
	game::play();

	//sets the entire board to null charaters
	init_board();

	//set player 1 to the host
	if (net.is_server())
	{
		player_sign = '1';
	}

	//set player to 2 if not the host
	else
	{
		player_sign = '2';
	}

	display_board(); //debugging

	//run until winner or tie
	while (gameState == 0)
	{




		//play makes his turn
		turn();

		display_board(); //debugging

		//check if they just won

		//switch the player turn
		if (playerturn == '1')
			playerturn = '2';
		else
			playerturn = '1';



	}

	//show the winner
	declareWinner();
}

/*************************************************************
* Function:init board
* Description:sets all the board charateristics
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void connectGame::init_board()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			board[i][j] = '\0';
		}
	}
}

/*************************************************************
* Function:barrier
* Description:this exists so that each game can wait for the other to make a move
* Input parameters:none
* Returns:none
* Preconditions:none
* Postconditions:none
*************************************************************/
void game::barrier()
{
	UserData data;
	net.send(data);
	data = net.recieve();
}
