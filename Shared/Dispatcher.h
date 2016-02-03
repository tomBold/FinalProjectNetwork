/*
 * Dispatcher.h
 *
 * Manages user requests.
 *
 *  Created on: Feb 3, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "MThread.h"
#include "TCPSocket.h"
#include <vector>
#include <map>
#include "ExtendedMultipleTCPSocketListener.h"
#include "Types.h"

using namespace std;

class Dispatcher: public MThread {
protected:
	map<ip_and_port, TCPSocket*> sockets;
	ExtendedMultipleTCPSocketListener* multiSocketListener;
	bool isRunning;
public:
	Dispatcher();
	virtual ~Dispatcher();

	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	virtual void addSocket(TCPSocket* socket) = 0;

	/*
	 * Delete socket by socket
	 */
	void deleteSocket(TCPSocket* socket);

	/*
	 * Delete socket by ip and port
	 */
	virtual void deleteSocket(ip_and_port ipAndPort) = 0;

	/**
	 * Retrieve commands from peers
	 */
	void run();

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	virtual void handleSocket(TCPSocket* socket) = 0;

	/*
	 * Get the dispatchers' sockets
	 */
	vector<TCPSocket*> getSockets();

	/**
	 * Check if the socket exists
	 */
	virtual bool isSocketExists(TCPSocket* socket) = 0;

	/**
	 * Disconnect a client
	 */
	virtual void disconnectClient(TCPSocket* socket) = 0;

	/**
	 * Shutdown
	 */
	virtual void shutdown() = 0;
};

#endif /* DISPATCHER_H_ */
