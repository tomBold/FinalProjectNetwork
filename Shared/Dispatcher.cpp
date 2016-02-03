/*
 * Dispatcher.cpp
 *
 * Manages user requests.
 *
 *  Created on: Feb 3, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "Dispatcher.h"

Dispatcher::Dispatcher() {
	// Initialization properties
	this->isRunning = true;
	this->multiSocketListener = new ExtendedMultipleTCPSocketListener();
	this->start();
}

Dispatcher::~Dispatcher() {
	delete this->multiSocketListener;

	// Delete the map sockets
	for (map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		delete it->second;
	}
}

/*
 * Delete socket by socket
 */
void Dispatcher::deleteSocket(TCPSocket* socket) {
	this->deleteSocket(socket->destIpAndPort());
}

/**
 * Retrieve commands from peers
 */
void Dispatcher::run() {
	while (this->isRunning) {
		// Gets the socket that ready
		TCPSocket* currSocket = this->multiSocketListener->listenToSocket();

		if (currSocket != NULL) {
			this->handleSocket(currSocket);
		}
	}
}

/*
 * Get the dispatchers' sockets
 */
vector<TCPSocket*> Dispatcher::getSockets() {
	vector<TCPSocket*> socketsVec;

	for (map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		socketsVec.push_back(it->second);
	}

	return socketsVec;
}
