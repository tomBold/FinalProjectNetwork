/*
 * TCPMessengerClient.h
 *
 * Handle the communication between the MCA app to the server.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef TCPMESSENGERCLIENT_H_
#define TCPMESSENGERCLIENT_H_

#include <strings.h>
#include <string.h>
#include "TCPSocket.h"
#include "UDPMessenger.h"
#include "MThread.h"
#include "ServerIO.h"
#include <map>
#include "TCPMessengerProtocol.h"
#include <iostream>
#include <string>

using namespace std;

class TCPMessengerClient: public MThread {
	string user;
	TCPSocket* socket;
	int status;
	string currentConversation;
	bool isRunning;
	UDPMessenger* udpMessenger;

	bool isLoggedIn();
	bool sendCommand(int command);
public:
	TCPMessengerClient();
	~TCPMessengerClient();

	/**
	 * Connect to server
	 */
	bool connect(string ip);

	/**
	 * Check if the user connected
	 */
	bool isConnected();

	/**
	 * Disconnect
	 */
	bool disconnect();

	/**
	 * Gets all users
	 */
	bool getAllUsers();

	/**
	 * Gets connected users
	 */
	bool getConnectedUsers();

	/**
	 * Gets all room names
	 */
	bool getAllRooms();

	/**
	 * Gets the rooms' users
	 */
	bool getRoomsUsers(string room);

	/**
	 * Login
	 */
	bool login(string name, string password);

	/**
	 * Register user
	 */
	bool registerUser(string name, string password);

	/**
	 * Gets the server ip
	 */
	string getServerIp();

	/**
	 * Get the current conversation
	 */
	string getConversation();

	/**
	 * Open a session
	 */
	bool openSession(string user);

	/**
	 * Join a room
	 */
	bool joinRoom(string room);

	/**
	 * Close room
	 */
	bool closeRoom(string room);

	/**
	 * Exit room
	 */
	bool exitRoom();

	/**
	 * Return if the user is in active broker
	 */
	bool isActiveBroker();

	/**
	 * Return if the user is in active room
	 */
	bool isActiveRoom();

	/**
	 * Close active session
	 */
	bool closeActiveSession();

	/**
	 * Send message
	 */
	bool send(string msg);

	/**
	 * Print the user status
	 */
	void printStatus();

	/**
	 * Run a command
	 */
	void run();

	/**
	 * Set the user status
	 */
	void setStatus(int status);
};

#endif /* TCPMESSENGERCLIENT_H_ */
