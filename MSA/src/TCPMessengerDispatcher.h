/*
 * TCPMessengerDispatcher.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#include <strings.h>
#include <map>
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerServer.h"
#include "Broker.h"

#ifndef TCPMESSENGERDISPATCHER_H_
#define TCPMESSENGERDISPATCHER_H_

class TCPMessengerDispatcher : public MThread{
	map<string, TCPSocket*> sockets;
	MultipleTCPSocketsListener* multiSocketListener;

public:
	TCPMessengerDispatcher();
	virtual ~TCPMessengerDispatcher();

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

	/*
	 * Print the sockets keys
	 */
	void printKeys();

	/**
	 * Retrieve messages from peers
	 */
	void run();

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	void handleSocket(TCPSocket* socket);

	/*
	 * Create a broker
	 */
	void createBroker(TCPSocket* firstSocket, TCPSocket* secondSocket);
	vector<TCPSocket*> getSockets();
	void createSession(TCPSocket* socket, string peer);
	bool isSocketExists(TCPSocket* socket);
	void createMultipleTCPSocketListener();
	void exit(TCPSocket* socket);
};

#endif /* TCPMESSENGERDISPATCHER_H_ */
