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

	void getUserAndPasswordFromSocket(TCPSocket* socket, string* name, string* password);

public:
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
	 * Delete socket by socket
	 */
	void deleteSocket(TCPSocket* socket);

	/*
	 * Delete socket by ip and port
	 */
	void deleteSocket(string ipAndPort);

	/**
	 * Retrieve commands from peers
	 */
	void run();

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	void handleSocket(TCPSocket* socket);

	/*
	 * Get the sockets
	 */
	vector<TCPSocket*> getSockets();

	/**
	 * Checl if the socket exists
	 */
	bool isSocketExists(TCPSocket* socket);

	/**
	 * Initiates the multiple tcp socket listener
	 */
	void createMultipleTCPSocketListener();

	/**
	 * Disconnect a client
	 */
	void disconnectClient(TCPSocket* socket);

	/**
	 * Client login
	 */
	void userLogin(TCPSocket* socket, string name);
};

#endif /* AUTHDISPATCHER_H_ */
