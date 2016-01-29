/*
 * TCPMessengerDispatcher.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#include "TCPMessengerDispatcher.h"
#include "TCPMessengerProtocol.h"

TCPMessengerDispatcher::TCPMessengerDispatcher() {
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->start();
}

TCPMessengerDispatcher::~TCPMessengerDispatcher() {
	delete this->multiSocketListener;

	// Delete the map sockets
	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		delete it->second;
	}
}

/*
 * Add socket to the map and the multiple tcp socket listener
 */
void TCPMessengerDispatcher::addSocket(TCPSocket* socket) {

	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);
}

/*
 * Delete socket by socket
 */
void TCPMessengerDispatcher::deleteSocket(TCPSocket* socket) {
	std::string destIpAndPort = socket->destIpAndPort();
	this->deleteSocket(destIpAndPort);
}

/*
 * Delete socket by key
 */
void TCPMessengerDispatcher::deleteSocket(string socketKey) {
	this->sockets.erase(this->sockets.find(socketKey));
}

/*
 * Print the sockets keys
 */
void TCPMessengerDispatcher::printKeys() {
	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		string key = it->first;
		cout << key << endl;
	}
}

/**
 * Retrieve messages from peers
 */
void TCPMessengerDispatcher::run() {
	while (true) {
		TCPSocket* currSocket = this->multiSocketListener->listenToSocket();

		if (currSocket != NULL) {
			this->handleSocket(currSocket);
		}
	}
}

/*
 * Create a broker
 */
void TCPMessengerDispatcher::createBroker(TCPSocket* firstSocket,
		TCPSocket* secondSocket) {
	Broker* b = new Broker(firstSocket, secondSocket, this);

	// Delete sockets
	this->deleteSocket(firstSocket);
	this->deleteSocket(secondSocket);

	this->createMultipleTCPSocketListener();
	b->start();
}

/*
 * Check if the socket exists in the sockets map
 */
bool TCPMessengerDispatcher::isSocketExists(TCPSocket* socket) {
	TCPSocket* sock = this->sockets[socket->destIpAndPort()];

	if (sock == NULL) {
		this->deleteSocket(sock);

		return false;
	}

	return true;
}

/*
 * Handle socket - gets the socket's command and executes it
 */
void TCPMessengerDispatcher::handleSocket(TCPSocket* socket) {
	if (!this->isSocketExists(socket)) {
		return;
	}

	int command = TCPMessengerServer::readCommandFromPeer(socket);

	switch (command) {
	case (CLOSE_SESSION_WITH_PEER): {
		cout << "Close session with peer" << endl;
		break;
	}
	case (OPEN_SESSION_WITH_PEER): {
		cout << "Open session with peer" << endl;
		string peer = TCPMessengerServer::readDataFromPeer(socket);
		createSession(socket, peer);
		break;
	}
	case (0): {
		this->exit(socket);
		break;
	}
	case (EXIT): {
		this->exit(socket);
		break;
	}
	case (SEND_MSG_TO_PEER): {
		cout << "Send message (not inside broker) to peer" << endl;
		string message = TCPMessengerServer::readDataFromPeer(socket);
		cout << message << endl;
		break;
	}
	case (SESSION_REFUSED): {
		cout << "Session refused" << endl;
		break;
	}
	case (SESSION_ESTABLISHED): {
		cout << "Session established" << endl;
		break;
	}
	default: {
		cout << "Command " << command << " not found" << endl;
		break;
	}
	}
}

/*
 * Get sockets vector
 */
vector<TCPSocket*> TCPMessengerDispatcher::getSockets() {
	vector<TCPSocket*> socketsVec;

	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		socketsVec.push_back(it->second);
	}

	return socketsVec;
}

/*
 * Create session
 */
void TCPMessengerDispatcher::createSession(TCPSocket* socket, string peer) {
	TCPSocket* peerSocket = this->sockets[peer];
	if (this->sockets[peer] == NULL) {
		this->deleteSocket(peer);
		TCPMessengerServer::sendCommandToPeer(socket, SESSION_REFUSED);
	} else if (socket->destIpAndPort() == peer) {
		TCPMessengerServer::sendCommandToPeer(socket, SESSION_REFUSED);
	} else {
		TCPSocket* peerSocket = this->sockets[peer];
		this->createBroker(socket, peerSocket);
	}
}

/*
 * Create the multiple TCP socket listener
 */
void TCPMessengerDispatcher::createMultipleTCPSocketListener() {
	delete this->multiSocketListener;
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->multiSocketListener->addSockets(this->getSockets());
}

/*
 * Handle exit socket
 */
void TCPMessengerDispatcher::exit(TCPSocket* socket) {
	cout << "Exit" << endl;
	this->deleteSocket(socket);
	socket->cclose();
	delete socket;
	this->createMultipleTCPSocketListener();
}

