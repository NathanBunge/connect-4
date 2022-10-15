//Nathan Bunge, Tom Arad, Jared Hartwig, Jake Bortles
//Lab Section 3 (exept Tom in lab 7)
//4/31/2020
//Cpts 122 Spring 2020
//Professor: Nadra
//PA 9: Connect 4
//Description: Classic 4 in a row game. Includes graphics using SFML. 
//Is able to connect to another game that is running if both comptuers are using the same internet
//WARNING: will not run on linux! (sorry)

#include "NetworkConnection.h"

/************************************************************
* Function: Packet& operator <<
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: an operator overload function which lets us bind a struct to a packet
* Input parameters: a reference to a packet and a reference to a struct of type UserData
* Returns: a packet that contains a struct
* Preconditions: none
* Postconditions: none
*************************************************************/

sf::Packet& operator <<(sf::Packet& packet, const UserData& data)
{
	return packet << data.col << data.user_name << data.command; // new
}

/************************************************************
* Function: Packet& operator >>
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: an operator overload function which allows us to read a packet whihc contains a struct
* Input parameters: a reference to a packet and a reference to a struct of type UserData
* Returns: a packet that contains a struct
* Preconditions: none
* Postconditions: none
*************************************************************/

sf::Packet& operator >>(sf::Packet& packet, UserData& data)
{
	return packet >> data.col >> data.user_name >> data.command; // new
}

/************************************************************
* Function: NetworkConnection (Constructor)
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: constructor initializes the server port
* Input parameters: none
* Returns: nothing
* Preconditions: none
* Postconditions: none
*************************************************************/

NetworkConnection::NetworkConnection()
{
	server_tcp_port = 3000;
}

/************************************************************
* Function: ~NetworkConnection (Destructor)
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: Destroys the instance of the class
* Input parameters: none
* Returns: nothing
* Preconditions: none
* Postconditions: none
*************************************************************/

NetworkConnection::~NetworkConnection()
{
}

/************************************************************
* Function: init
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: checks if the host is the client or the server and then calls a function which initializes the host depending on which one it is
* Input parameters: none
* Returns: nothing
* Preconditions: none
* Postconditions: none
*************************************************************/

void NetworkConnection::init()
{
	string is_server_string;

	// asking the user wether he is the client or the server
	do
	{
		cout << "Is this computer the server(yes/no): ";
		getline(cin, is_server_string);
		for_each(is_server_string.begin(), is_server_string.end(), [](char& c)
			{
				c = ::toupper(c);
			});
	} while (is_server_string != "YES" && is_server_string != "NO");

	_is_server = (is_server_string == "YES");

	if (_is_server) // host is the server
	{
		init_server(); // initializes the server host
	}
	else
	{
		init_client(); // intializes the client host
	}
}

/************************************************************
* Function: send
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: sends a struct containing information to the other host
* Input parameters: A struct of type UserData
* Returns: nothing
* Preconditions: none
* Postconditions: none
*************************************************************/

void NetworkConnection::send(UserData data)
{
	Packet packet;
	packet << data; // binds struct to packet
	socket.send(packet); // send packet to other host
}

/************************************************************
* Function: recieve
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: recieves information from the other host and returns a struct
* Input parameters: none
* Returns: Struct of type UserData
* Preconditions: none
* Postconditions: none
*************************************************************/

UserData NetworkConnection::recieve()
{
	UserData data;
	Packet packet;
	socket.receive(packet); // waiting until packet is recieved
	if (packet >> data) // checking if packet was recieved succesfully and converting the info into the struct
	{
		// cout << "packet read succesfully" << endl;
	}
	else
	{
		cout << "Failed to read packet" << endl;
		system("pause");
		exit(1);
	}
	return data; // returning the struct with the info
}

/************************************************************
* Function: is_server
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: checks the global variable "_is_server"'s value, returns true or false
* Input parameters: none
* Returns: true or false
* Preconditions: none
* Postconditions: none
*************************************************************/

bool NetworkConnection::is_server()
{
	return _is_server;
}

/************************************************************
* Function: init_server
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: waits for the client to connect to the server
* Input parameters: none
* Returns: none
* Preconditions: none
* Postconditions: none
*************************************************************/

void NetworkConnection::init_server()
{
	server_address = IpAddress::getLocalAddress(); // getting the local ip adress
	cout << "Please connect the client to this address: " << server_address.toString() << endl; // printing instructions to screen
	if (server_listener.listen(server_tcp_port) != Socket::Done) // checking if the binding of the socket was not succesful, if so exiting
	{
		cout << "Error: Server Failed To Bind Socket" << endl;
		exit(1);
	}
	cout << "Waiting for client to connect..." << endl;
	if (server_listener.accept(socket) != Socket::Done) // checking if the connection was established, if not exiting
	{
		cout << "Server Failed To Accept Connection" << endl;
		exit(1);
	}
	cout << "Server Succesfully Accepted Connection" << endl; // printing success message
}

/************************************************************
* Function: init_client
* Date Created: 4/29/2020
* Date Last Modified: 4/29/2020
* Description: uses the server address to connect the client with the server
* Input parameters: none
* Returns: none
* Preconditions: none
* Postconditions: none
*************************************************************/

void NetworkConnection::init_client()
{
	string server_address_string;
	cout << "Please enter server address: "; // cout printing instructions
	getline(cin, server_address_string); // reading user input
	server_address = IpAddress(server_address_string);
	Socket::Status status = socket.connect(server_address, server_tcp_port); // connecting the client to the server
	if (status != Socket::Done) // checking if connection was not succesful, if so, exiting
	{
		cout << "Client Failed To Connect" << endl;
		exit(1);
	}
	cout << "Client connected succesfully" << endl; // prints success message
}

