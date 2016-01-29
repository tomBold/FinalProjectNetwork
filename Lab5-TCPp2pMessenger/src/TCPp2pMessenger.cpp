//============================================================================
// Name        : TCPp2pMessenger.h
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : TCP p2p Messenger application
//============================================================================

#include <pthread.h>
#include "TCPSocket.h"
#include "TCPp2pMessenger.h"
#include <cstdlib>

using namespace std;

/**
 * Construct a TCP server socket, opens a thread that receive all incoming connection
 */
TCPp2pMessenger::TCPp2pMessenger() {
	// Initialize class properties
	p2pServer = new P2PServer(1327);
	p2pServer->start();
	p2pClient = NULL;

	sleep(1); //give a chance for the TCP server to start
}

/**
 * Open a connection to the given IP address and once the connection is established it will open a thread
 * to read incoming messages from the socket
 */
bool TCPp2pMessenger::open(string peerIp) {
	// If the client socket is already connected, disconnect it and free all related resources
	closeSession();

	// Create a new client socket and connect it to the given IP
	p2pClient = new P2PClient(peerIp, 1327);

	// Start a thread that reads incoming messages from the socket
	p2pClient->start();

	return false;
}

/**
 * Close an open session
 */
bool TCPp2pMessenger::closeSession() {
	// If the client socket is already connected, disconnect it and free all related resources
	if (isActiveClientSession()) {
		this->p2pClient->sock->cclose();
		this->p2pClient->waitForThread();

		delete this->p2pClient;
		this->p2pClient = NULL;
	}

	return true;
}

/**
 * send a message on the client open connection
 * The message is sent in the form: [message length - 4 bytes][message data]
 */
void TCPp2pMessenger::send(string msg) {
	if (!this->isActiveClientSession()) {
		cout << "Not connected to any server" << endl;
		return;
	}
	// Send message on the client socket
	int len = std::atoi(msg.substr(0, 4).c_str());
	this->p2pClient->sock->send(msg.substr(4).c_str(), len);
}

/**
 * send a message on the server socket incoming connection
 * The message is sent in the form: [message length - 4 bytes][message data]
 */
void TCPp2pMessenger::reply(string msg) {
	if (!this->isActivePeerSession()) {
		cout << "Not connected to any peer" << endl;
		return;
	}
	// Send message on the server socket
	int len = std::atoi(msg.substr(0, 4).c_str());
	this->p2pServer->sockSecond->send(msg.substr(4).c_str(), len);
}

/**
 * close all resources / sockets
 */
void TCPp2pMessenger::close() {
	// Close all sockets and threads
	if (this->p2pServer != NULL) {
		delete this->p2pServer;
	}

	if (this->p2pClient != NULL) {
		delete this->p2pClient;
	}
}

/**
 * return  true if the client socket is connected
 */
bool TCPp2pMessenger::isActiveClientSession() {
	return this->p2pClient != NULL;
}

/**
 * return true if there is an open incoming connection
 */
bool TCPp2pMessenger::isActivePeerSession() {
	return this->p2pServer != NULL && this->p2pServer->sockSecond != NULL;
}

