/*
 * AuthDispatcher.h
 *
 * This class is responsible of handling all the unauthenticated users
 * by letting them authenticate or register.
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
#include "Types.h"

using namespace std;

class TCPMessengerServer;

class AuthDispatcher: public Dispatcher {
private:
	TCPMessengerServer* tcpMessengerServer;

	/**
	 * Read user and password from socket and put them in the
	 * respective out parameters.
	 */
	void getUserAndPasswordFromSocket(TCPSocket* socket, user_name* name,
			string* password);

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
	void deleteSocket(ip_and_port ipAndPort);

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
	 * Authenticate user
	 */
	void userLogin(TCPSocket* socket, user_name name);

	/**
	 * Shutdown and disconnect every user
	 */
	void shutdown();
};

#endif /* AUTHDISPATCHER_H_ */
