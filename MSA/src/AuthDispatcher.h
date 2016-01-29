/*
 * AuthDispatcher.h
 *
 *  Created on: Jan 29, 2016
 *      Author: user
 */

#ifndef AUTHDISPATCHER_H_
#define AUTHDISPATCHER_H_

#pragma once

#include "MThread.h"
#include <strings.h>
#include "TCPSocket.h"
#include <map>
#include "MultipleTCPSocketsListener.h"
#include "Utilities/Users.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerServer.h"

using namespace Utilities;
using namespace std;

class TCPMessengerServer;

class AuthDispatcher: public MThread {
private:
	map<string, TCPSocket*> sockets;
	MultipleTCPSocketsListener* multiSocketListener;
	TCPMessengerServer* tcpMessengerServer;

public:
	AuthDispatcher(TCPMessengerServer* tcpMessengerServer);
	virtual ~AuthDispatcher();
	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Delete socket by socket
	 */
	void deleteSocket(TCPSocket* socket);

	/*
	 * Delete socket by key
	 */
	void deleteSocket(string socketKey);

	/**
	 * Retrieve messages from peers
	 */
	void run();

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	void handleSocket(TCPSocket* socket);

	vector<TCPSocket*> getSockets();
	bool isSocketExists(TCPSocket* socket);
	void createMultipleTCPSocketListener();
	void exit(TCPSocket* socket);
	void userLogin(TCPSocket* socket, string name);
};

#endif /* AUTHDISPATCHER_H_ */
