/*
 * TCPMessengerServer.h
 *
 * Listens to the port and moves peers to their dispatchers.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#pragma once

#ifndef TCPMESSENGERSERVER_H__
#define TCPMESSENGERSERVER_H__
#define INT_BYTE_LENGTH 4

#include <strings.h>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include "MThread.h"
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerDispatcher.h"
#include "AuthDispatcher.h"
#include "ServerIO.h"
#include "Types.h"

class AuthDispatcher;

using namespace std;

class TCPMessengerServer: public MThread {
	TCPSocket* serverSocket;
	AuthDispatcher* authDispatcher;
	TCPMessengerDispatcher* dispatcher;

public:
	/**
	 * Build the Messenger server
	 */
	TCPMessengerServer();
	~TCPMessengerServer();

	/**
	 * Print out the list of connected clients (for debug)
	 */
	void listPeers();

	/**
	 * Close the server and disconnect all the users
	 */
	void close();

	/**
	 * Listen and accept peers
	 */
	void run();

	/**
	 * Handle user authenticated
	 */
	void userLogin(TCPSocket* peer, user_name name);

	/**
	 * Tells whether a user is connected
	 */
	bool isUserConnected(user_name name);

	/**
	 * A string representation of all the connected users
	 */
	string getConnectUsers();

	/**
	 * A string representation of all the brokers
	 */
	string getAllBrokers();

	/**
	 * A string representation of all the rooms
	 */
	string getRoomsNames();

	/**
	 * A string representation of all the users in a certain room
	 */
	string getRoomsUsers(room_name roomName);
};
#endif
