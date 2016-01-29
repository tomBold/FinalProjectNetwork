#include "TCPMessengerClient.h"
#include "TCPMessengerProtocol.h"
#include <string.h>
#include <stdint.h>

using namespace std;

/**
 * initialize all properties
 */
TCPMessengerClient::TCPMessengerClient() {
	this->serverSocket = NULL;
	this->hasConnection = false;
	this->reader = NULL;
}

/**
 * connect to the given server ip (the port is defined in the protocol header file)
 */
bool TCPMessengerClient::connect(string ip) {
	if (this->isConnected()) {
		cout << "Already connected to a server. press d to disconnect." << endl;
		return false;
	}
	this->serverSocket = new TCPSocket(ip, MSNGR_PORT);
	this->reader = new AsyncTCPMessengerReader(this->serverSocket, this);
	reader->start();
	return true;
}

/**
 * Handle receiving a message from the server
 */
void TCPMessengerClient::onServerMessage(int cmd, char* data) {
	switch (cmd) {
	case (SEND_MSG_TO_PEER): {
		cout << "message: " << data << endl;
		break;
	}
	case (SESSION_ESTABLISHED): {
		cout << "connection established" << endl;
		this->hasConnection = true;
		break;
	}
	case (SESSION_REFUSED): {
		cout << "connection refused" << endl;
		this->hasConnection = false;
		break;
	}
	case (OPEN_SESSION_WITH_PEER): {
		cout << "open session with peer " << data << endl;
		this->hasConnection = true;
		break;
	}
	case (CLOSE_SESSION_WITH_PEER): {
		cout << "close session with peer" << endl;
		this->hasConnection = false;
		break;
	}
	default: {
		cout << "unknown command " << cmd << endl;
		break;
	}
	}
}

/**
 * return true if connected to the server
 */
bool TCPMessengerClient::isConnected() {
	// TODO: what happens when server dies?
	return this->serverSocket != NULL;
}

/**
 * disconnect from messenger server
 */
bool TCPMessengerClient::disconnect() {
	this->serverSocket->cclose();
	this->reader->waitForThread();
	delete this->serverSocket;
	delete this->reader;
	this->serverSocket = NULL;
	this->reader = NULL;
	return true;
}

/**
 * open session with the given peer address (ip:port)
 */
bool TCPMessengerClient::open(string address) {
	return this->sendCommand(OPEN_SESSION_WITH_PEER, address);
}

/**
 * return true if a session is active
 */
bool TCPMessengerClient::isActiveClientSession() {
	return this->hasConnection;
}

/**
 * close active session
 */
bool TCPMessengerClient::closeActiveSession() {
	this->hasConnection = false;
	return this->sendCommand(CLOSE_SESSION_WITH_PEER);
}

/**
 * send the given message to the connected peer
 */
bool TCPMessengerClient::send(string msg) {
	return this->sendCommand(SEND_MSG_TO_PEER, msg);
}

/**
 * Sends a command to the remote server
 */
bool TCPMessengerClient::sendCommand(int command, string data) {
	if (isConnected()) {
		int length = 0;
		char* cmd = TCPMessengerClient::createMessage(command, data, length);
		return this->serverSocket->send(cmd, length);
	}

	cout << "not connected to server" << endl;
	return false;
}

/**
 * Sends a command to the remote server
 */
bool TCPMessengerClient::sendCommand(int command) {
	return this->sendCommand(command, "");
}

/**
 * Converts an int to a byte array
 */
char* TCPMessengerClient::intToByteArray(int number) {
	char* arr = new char[4];
	arr[0] = (number >> 24) & 0xFF;
	arr[1] = (number >> 16) & 0xFF;
	arr[2] = (number >> 8) & 0xFF;
	arr[3] = number & 0xFF;

	return arr;
}

/**
 * Converts a byte array to int
 */
int TCPMessengerClient::byteArrayToInt(char* str) {
	int i = str[0] << 8;
	int j = str[1] << 16;
	int k = str[2] << 24;
	char l = str[3];

	return i | j | k | l;
}

/**
 * Creates a message byte array from the data received
 * with the length as an output parameter
 */
char* TCPMessengerClient::createMessage(int command, string data,
		int& outLength) {
	char* cmd = intToByteArray(command);
	int length = data.length();
	if (length > 0) {
		char* dataLength = intToByteArray(length);
		const char* dataArr = data.c_str();
		outLength = length + 4 + 4;
		char* result = new char[outLength];

		for (int i = 0; i < outLength; i++) {
			if (i < 4) {
				result[i] = cmd[i];
			} else if (i < 8) {
				result[i] = dataLength[i - 4];
			} else {
				result[i] = dataArr[i - 8];
			}
		}

		return result;
	} else {
		outLength = 4;
		return cmd;
	}
}

/**
 * Delete the sockets and the threads
 */
TCPMessengerClient::~TCPMessengerClient() {
	if (this->serverSocket != NULL) {
		delete serverSocket;
	}

	if (this->reader != NULL) {
		delete reader;
	}
}
