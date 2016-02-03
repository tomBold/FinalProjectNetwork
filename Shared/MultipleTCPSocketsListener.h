/*
 * MultipleTCPSocketsListener.h
 *
 * Provides an interface to listen to multiple sockets and get the first socket
 * that can be read from.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef MULTIPLETCPSOCKETSLISTENER_H_
#define MULTIPLETCPSOCKETSLISTENER_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/select.h>
#include "TCPSocket.h"
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

class MultipleTCPSocketsListener {
	/*
	 * Sockets to listen to
	 */
	vector<TCPSocket*> sockets;

public:
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
};


#endif /* MULTIPLETCPSOCKETSLISTENER_H_ */
