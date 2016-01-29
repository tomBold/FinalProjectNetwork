/*
 * Broker.cpp
 *
 *  Created on: Dec 18, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#include "Broker.h"
#include "TCPMessengerProtocol.h"

Broker::Broker() :
		Broker(NULL, NULL, NULL) {
}

Broker::Broker(TCPSocket* initiativeSocket, TCPSocket* recvSocket,
		TCPMessengerDispatcher* dispatcher) {
	this->areBothPeersConnected = true;
	this->firstSocket = initiativeSocket;
	this->secondSocket = recvSocket;
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->multiSocketListener->addSocket(this->firstSocket);
	this->multiSocketListener->addSocket(this->secondSocket);
	this->dispatcher = dispatcher;

	cout << "Open session between " << initiativeSocket->destIpAndPort()
			<< " and " << recvSocket->destIpAndPort() << endl;
	TCPMessengerServer::sendCommandToPeer(initiativeSocket,
	SESSION_ESTABLISHED);
	TCPMessengerServer::sendCommandToPeer(recvSocket, OPEN_SESSION_WITH_PEER);
	TCPMessengerServer::sendDataToPeer(recvSocket,
			initiativeSocket->destIpAndPort());
}

Broker::~Broker() {
	delete this->multiSocketListener;
}

/**
 * Run the messages between peers
 */
void Broker::run() {
	this->areBothPeersConnected = true;

	while (areBothPeersConnected) {

		// Find who from the sockets will send the next command
		TCPSocket* socket = this->multiSocketListener->listenToSocket();

		// If no one sends a command, try again
		if (socket == NULL) {
			continue;
		}

		int command = TCPMessengerServer::readCommandFromPeer(socket);
		switch (command) {
		// when a SIG_TERM happens, the client sends 0
		case (CLOSE_SESSION_WITH_PEER): {
			areBothPeersConnected = false;
			this->dispatcher->closeBroker(this);

			break;
		}
		default: {
			this->dispatcher->handleSocketCommand(socket, command);
		}
		}
	}
}

