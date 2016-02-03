/*
 * AuthDispatcher.h
 * This class is responsible of authenticating users or registering them.
 *
 *  Created on: Jan 29, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef AUTHDISPATCHER_H_
#define AUTHDISPATCHER_H_

#include "MThread.h"
#include <strings.h>
#include "TCPSocket.h"
#include <map>
#include "ExtendedMultipleTCPSocketListener.h"
#include "Utilities/Users.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerServer.h"
#include "ServerIO.h"
#include "Dispatcher.h"

using namespace std;

class TCPMessengerServer;

class AuthDispatcher: public Dispatcher {
private:
	TCPMessengerServer* tcpMessengerServer;

	void getUserAndPasswordFromSocket(TCPSocket* socket, string* name, string* password);

public:
	 using Dispatcher::deleteSocket;
	/*
	 * CTOR
	 */
	AuthDispatcher(TCPMessengerServer* tcpMessengerServer);

	/**
	 * DTOR
	 */
	virtual ~AuthDispatcher();

	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Delete socket by ip and port
	 */
	void deleteSocket(string ipAndPort);

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	void handleSocket(TCPSocket* socket);

	/**
	 * Check if the socket exists
	 */
	bool isSocketExists(TCPSocket* socket);

	/**
	 * Disconnect a client
	 */
	void disconnectClient(TCPSocket* socket);

	/**
	 * Client login
	 */
	void userLogin(TCPSocket* socket, string name);

	/**
	 * Shutdown
	 */
	void shutdown();
};

#endif /* AUTHDISPATCHER_H_ */
