/*
 * TCPMessengerServer.h
 * This class holds is responsible for getting new connections and sends them
 * to their dispatchers
 *
 *  Created on: Dec 16, 2015
 *      Author: Tom Boldan and Gal Schlezinger
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

class AuthDispatcher;

using namespace std;

class TCPMessengerServer : public MThread{
	TCPSocket* serverSocket;
	AuthDispatcher* authDispatcher;

public:
	/**
	 * Build the Messenger server
	 */
	TCPMessengerDispatcher* dispatcher;
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
};
#endif
