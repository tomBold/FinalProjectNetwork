#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

/***********************   TCPMessengerServer implementation ******************************/

TCPMessengerServer::TCPMessengerServer() {
	this->serverSocket = new TCPSocket(MSNGR_PORT);
	this->dispatcher = new TCPMessengerDispatcher();
	this->authDispatcher = new AuthDispatcher(this);
	this->start();
}

/*
 * Close the server
 */
void TCPMessengerServer::close() {
	this->serverSocket->cclose();
	this->dispatcher->shutdown();
	this->authDispatcher->shutdown();
}

TCPMessengerServer::~TCPMessengerServer() {
	delete this->serverSocket;
	delete this->dispatcher;
	delete this->authDispatcher;
}

/*
 * Print out the list of connected clients (for debug)
 */
void TCPMessengerServer::listPeers() {
	this->dispatcher->printKeys();
}

/*
 * Listen and add sockets to the dispatcher
 */
void TCPMessengerServer::run() {
	while (this->serverSocket != NULL) {
		TCPSocket* listener = this->serverSocket->listenAndAccept();
		this->authDispatcher->addSocket(listener);
	}
}

void TCPMessengerServer::userLogin(TCPSocket* peer, string name) {
	ServerIO::sendCommandToPeer(peer, PORT_INIT_REQ);

	int command = ServerIO::readCommandFromPeer(peer);

	if (command != PORT_INIT_RES) {
		if (command == DISCONNECT_FROM_SERVER_REQ || command == 0) {
			peer->cclose();
			delete peer;

			return;
		}

		ServerIO::sendCommandToPeer(peer, PORT_INIT_FAILED);
		this->authDispatcher->addSocket(peer);

		return;
	}
	ServerIO::sendCommandToPeer(peer, SUCCESSFULLY_LOGIN_RES);
	ServerIO::sendDataToPeer(peer, name);

	this->dispatcher->addSocket(peer, name,
			ServerIO::readDataFromPeer(peer));
}

