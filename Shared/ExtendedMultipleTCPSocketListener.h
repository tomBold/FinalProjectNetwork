/*
 * ExtendedMultipleTCPSocketListener.h
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef EXTENDEDMULTIPLETCPSOCKETLISTENER_H_
#define EXTENDEDMULTIPLETCPSOCKETLISTENER_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include "TCPSocket.h"
#include <set>
#include "MultipleTCPSocketsListener.h"

using namespace std;

class ExtendedMultipleTCPSocketListener {
	MultipleTCPSocketsListener* multipleTCPSocketsListener;
	set<TCPSocket*> sockets;
public:
	ExtendedMultipleTCPSocketListener();
	virtual ~ExtendedMultipleTCPSocketListener();

	/*
	 * Add the given socket to the socket list to be listen on
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Set the given sockets to the socket list to be listen on
	 */
	void addSockets(vector<TCPSocket*> socketVec);

	/*
	 * This method checks to see if any of the previous given sockets are ready for reading
	 * It returns one of the Sockets that are ready.
	 */
	TCPSocket* listenToSocket();

	/**
	 * This method remove socket
	 */
	void removeSocket(TCPSocket* socket);
};

#endif /* EXTENDEDMULTIPLETCPSOCKETLISTENER_H_ */
