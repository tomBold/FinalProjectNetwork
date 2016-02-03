//============================================================================
// Name        : TCP Messenger Server
// Author      : Tom Boldan and Gal Schlezinger
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
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

class AuthDispatcher;

using namespace std;

class TCPMessengerServer : public MThread{
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
	 * print out the list of connected clients (for debug)
	 */
	void listPeers();

	/**
	 * close the server
	 */
	void close();

	void run();

	void userLogin(TCPSocket* peer, string name);

	bool isUserConnected(string name);

	string getConnectUsers();

	string getAllBrokers();

	string getRoomsNames();

	string getRoomsUsers(string roomName);
};
#endif
