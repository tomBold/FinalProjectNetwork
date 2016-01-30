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
}

TCPMessengerServer::~TCPMessengerServer() {
	delete this->serverSocket;
	delete this->dispatcher;
	delete this->authDispatcher;
}

/*
 * Read command from peer
 */
int TCPMessengerServer::readCommandFromPeer(TCPSocket* peer) {
	int result;

	// Read a command from socket
	peer->recv((char*) &result, INT_BYTE_LENGTH);

	return ntohl(result);
}

/*
 * Read data from peer
 */
string TCPMessengerServer::readDataFromPeer(TCPSocket* peer) {
	// Read a string from socket
	int length = readCommandFromPeer(peer);

	// Receive the incoming message
	char buffer[length];
	memset((void*) buffer, 0, length);

	int rc = peer->recv(buffer, length);

	if (rc < 0) {
		perror("Error receiving socket");

		return "";
	}

	string str = buffer;

	return str.substr(0, length);
}

/*
 * Send command to peer
 */
void TCPMessengerServer::sendCommandToPeer(TCPSocket* peer, int command) {
	// Send command to socket
	int msgLen = htonl(command);
	peer->send((char*) &msgLen, INT_BYTE_LENGTH);
}

/**
 * Send data to peer
 */
void TCPMessengerServer::sendDataToPeer(TCPSocket* peer, string msg) {
	// Send the message back to the client
	int msgLen = htonl(msg.length());
	peer->send((char*) &msgLen, INT_BYTE_LENGTH);
	peer->send(msg.data(), msg.length());
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
	this->dispatcher->addSocket(peer, name);
	TCPMessengerServer::sendCommandToPeer(peer, SUCCESSFULY_LOGIN_RES);
}

