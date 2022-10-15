//Nathan Bunge, Tom Arad, Jared Hartwig, Jake Bortles
//Lab Section 3 (exept Tom in lab 7)
//4/31/2020
//Cpts 122 Spring 2020
//Professor: Nadra
//PA 9: Connect 4
//Description: Classic 4 in a row game. Includes graphics using SFML. 
//Is able to connect to another game that is running if both comptuers are using the same internet
//WARNING: will not run on linux! (sorry)

#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include <iostream>
#include <algorithm>
#include <SFML\Network.hpp> // includes the SFML library which contains networking functions

using namespace sf;
using namespace std;

enum { // new
	COMMAND_CONTINUE,
	COMMAND_TERMINATE
};

struct UserData // creating a struct with predeclared variables of sfml 
{
	Int16 col;
	Int16 command = COMMAND_CONTINUE; // new
	sf::String user_name;
};
sf::Packet& operator <<(sf::Packet& packet, const UserData& data); // an operator overload function which lets us bind a struct to a packet
sf::Packet& operator >>(sf::Packet& packet, UserData& data); // an operator overload function which allows us to read a packet whihc contains a struct


class NetworkConnection
{
public:

	NetworkConnection(); // constructor initializes the server port
	~NetworkConnection(); // destructor is defualt

	void init(); // checks if the host is the client or the server and then calls an init function depending on which one it is
	void send(UserData); // sends a struct containing information to the other host
	UserData recieve(); // recieves information from the other host and returns a struct
	bool is_server(); // checks if the host is the server, returns true or false
	TcpSocket socket; // initializing a socket;

private:

	IpAddress server_address; // intializing several components of the networking functions for later use;
	bool _is_server;
	int server_tcp_port;
	TcpListener server_listener;


	void init_server(); // waits for the client to connect to the server
	void init_client(); // uses the server address to connect the client with the server 
};

#endif