/*
 * Broker.cpp
 *
 *  Created on: Dec 18, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#include "Broker.h"

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
		TCPSocket* socket = this->multiSocketListener->listenToSocket(2);

		// If no one sends a command, try again
		if (socket == NULL) {
			continue;
		}

		int command = TCPMessengerServer::readCommandFromPeer(socket);
		switch (command) {
		case (SEND_MSG_TO_PEER): {
			string msg = TCPMessengerServer::readDataFromPeer(socket);
			TCPSocket* peer;

			// Find out our message destination
			if (this->firstSocket->destIpAndPort() == socket->destIpAndPort()) {
				peer = this->secondSocket;
			} else {
				peer = this->firstSocket;
			}

			// Sends a message
			TCPMessengerServer::sendCommandToPeer(peer, SEND_MSG_TO_PEER);
			TCPMessengerServer::sendDataToPeer(peer, msg);
			break;
		}
		// when a SIG_TERM happens, the client sends 0
		case (0): {
			this->close();
			this->dispatcher->deleteSocket(socket);

			break;
		}
		case (EXIT): {
			this->close();
			this->dispatcher->deleteSocket(socket);

			break;
		}
		case (CLOSE_SESSION_WITH_PEER): {
			this->close();
			break;
		}
		case (OPEN_SESSION_WITH_PEER): {
			string peer = TCPMessengerServer::readDataFromPeer(socket);
			this->close();
			this->dispatcher->createSession(socket, peer);
			break;
		}
		}
	}
}

/*
 * Close the broker and reattach the sockets to the dispatcher
 */
void Broker::close() {
	this->areBothPeersConnected = false;
	this->dispatcher->addSocket(this->firstSocket);
	this->dispatcher->addSocket(this->secondSocket);
}
