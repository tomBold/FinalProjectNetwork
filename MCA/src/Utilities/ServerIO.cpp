/*
 * ServerIO.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: user
 */

#include "ServerIO.h"

/*
 * Read command from peer
 */
int ServerIO::readCommandFromPeer(TCPSocket* peer) {
	int result;

	// Read a command from socket
	peer->recv((char*) &result, INT_BYTE_LENGTH);

	return ntohl(result);
}

/*
 * Read data from peer
 */
string ServerIO::readDataFromPeer(TCPSocket* peer) {
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
void ServerIO::sendCommandToPeer(TCPSocket* peer, int command) {
	// Send command to socket
	int msgLen = htonl(command);
	peer->send((char*) &msgLen, INT_BYTE_LENGTH);
}

/**
 * Send data to peer
 */
void ServerIO::sendDataToPeer(TCPSocket* peer, string msg) {
	// Send the message back to the client
	int msgLen = htonl(msg.length());
	peer->send((char*) &msgLen, INT_BYTE_LENGTH);
	peer->send(msg.data(), msg.length());
}

