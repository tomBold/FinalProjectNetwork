/*
 * ExtendedMultipleTCPSocketListener.cpp
 *
 * Extending the multiple TCP socket listener.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "ExtendedMultipleTCPSocketListener.h"

ExtendedMultipleTCPSocketListener::ExtendedMultipleTCPSocketListener() {
	this->multipleTCPSocketsListener = new MultipleTCPSocketsListener();

}

ExtendedMultipleTCPSocketListener::~ExtendedMultipleTCPSocketListener() {
	delete this->multipleTCPSocketsListener;
}

/*
 * Add the given socket to the socket list to be listen on.
 */
void ExtendedMultipleTCPSocketListener::addSocket(TCPSocket* socket) {
	this->sockets.insert(socket);
	this->multipleTCPSocketsListener->addSocket(socket);
}

/*
 * Set the given sockets to the socket list to be listen on.
 */
void ExtendedMultipleTCPSocketListener::addSockets(
		vector<TCPSocket*> socketVec) {
	vector<TCPSocket*>::iterator it;

	for (it = socketVec.begin(); it != socketVec.end(); it++) {
		TCPSocket* socket = *it;
		this->addSocket(socket);
	}
}

/*
 * This method checks to see if any of the previous given sockets are ready for reading
 * It returns one of the Sockets that are ready.
 */
TCPSocket* ExtendedMultipleTCPSocketListener::listenToSocket() {
	return this->multipleTCPSocketsListener->listenToSocket();
}

/**
 * This method remove socket from the socket list to be listen on.
 */
void ExtendedMultipleTCPSocketListener::removeSocket(TCPSocket* socket) {
	MultipleTCPSocketsListener* old = this->multipleTCPSocketsListener;
	MultipleTCPSocketsListener* newListener = new MultipleTCPSocketsListener();

	this->sockets.erase(socket);

	set<TCPSocket*>::iterator it;
	for (it = this->sockets.begin(); it != this->sockets.end(); it++) {
		TCPSocket* socket = *it;
		newListener->addSocket(socket);
	}

	this->multipleTCPSocketsListener = newListener;

	delete old;
}

