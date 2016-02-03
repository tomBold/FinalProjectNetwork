/*
 * Broker.h
 * This class handles communication peer to peer
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

	ServerIO::sendCommandToPeer(this->firstSocket,
	SESSION_ESTABLISHED);
	ServerIO::sendDataToPeer(this->firstSocket,
			this->dispatcher->getUserByPeerIp(this->secondSocket->destIpAndPort()));
	ServerIO::sendCommandToPeer(this->secondSocket,
	SESSION_ESTABLISHED);
	ServerIO::sendDataToPeer(this->secondSocket,
			this->dispatcher->getUserByPeerIp(this->firstSocket->destIpAndPort()));

	this->sendNewDest();
}

Broker::~Broker() {
	delete this->multiSocketListener;
}

/**
 * Send the new messages destination
 */
void Broker::sendNewDest() {
	ServerIO::sendCommandToPeer(this->firstSocket,
	NEW_MESSAGE_DST_RES);
	ServerIO::sendDataToPeer(this->firstSocket,
			this->dispatcher->getUserP2PAddress(this->secondSocket));

	ServerIO::sendCommandToPeer(this->secondSocket,
	NEW_MESSAGE_DST_RES);
	ServerIO::sendDataToPeer(this->secondSocket,
			this->dispatcher->getUserP2PAddress(this->firstSocket));
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

		int command = ServerIO::readCommandFromPeer(socket);
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

	this->dispatcher->deleteBroker(this);
}

/**
 * Check if the user is in the broker
 */
bool Broker::isUserInBroker(string ip) {
	return this->firstSocket->destIpAndPort() == ip
			|| this->secondSocket->destIpAndPort() == ip;
}
