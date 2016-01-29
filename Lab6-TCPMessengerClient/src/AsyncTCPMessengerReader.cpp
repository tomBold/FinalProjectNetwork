/*
 * AsyncTCPMessengerReader.cpp
 *
 *  Created on: Dec 11, 2015
 *      Author: user
 */

#include "AsyncTCPMessengerReader.h"

AsyncTCPMessengerReader::AsyncTCPMessengerReader() {
	this->client = NULL;
	this->socket = NULL;
}

AsyncTCPMessengerReader::AsyncTCPMessengerReader(TCPSocket* socket,
		TCPMessengerClient* client) {
	this->client = client;
	this->socket = socket;
}

AsyncTCPMessengerReader::~AsyncTCPMessengerReader() {
	// TODO Auto-generated destructor stub
}

void AsyncTCPMessengerReader::run() {
	char* cmd = new char[4];
	char* dataLength = new char[4];
	char* buffer;
	while (socket->recv(cmd, 4) > 0) {
		int commandNumber = TCPMessengerClient::byteArrayToInt(cmd);
		if (commandNumber == 2 || commandNumber == 4) {
			socket->recv(dataLength, 4);
			int length = TCPMessengerClient::byteArrayToInt(dataLength);
			buffer = new char[length];
			socket->recv(buffer, length);

			client->onServerMessage(commandNumber, buffer);

			delete buffer;
		} else {
			client->onServerMessage(commandNumber, NULL);
		}
	}
	delete cmd;
	delete dataLength;
}

