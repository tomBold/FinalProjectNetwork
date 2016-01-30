/*
 * TCPMessengerClient.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: user
 */

#include "TCPMessengerClient.h"

TCPMessengerClient::TCPMessengerClient() {
	this->socket = NULL;
	this->status = DISCONNECTED;
	this->udpMessenger = NULL;
	this->currentConversation = "";
	this->isRunning = false;
	this->user = "";
}

TCPMessengerClient::~TCPMessengerClient() {
	if (this->udpMessenger != NULL) {
		delete this->udpMessenger;
	}

	if (this->socket != NULL) {
		delete this->socket;
	}

	this->status = DISCONNECTED;
}

bool TCPMessengerClient::connect(string ip) {
	if (this->isConnected()) {
		cout << "Already connected to a server. press d to disconnect." << endl;
		return false;
	}

	this->socket = new TCPSocket(ip, MSNGR_PORT);
	this->udpMessenger = new UDPMessenger(this);
	this->status = AUTH;
	this->start();

	return true;
}

bool TCPMessengerClient::isConnected() {
	return this->status != DISCONNECTED;
}

bool TCPMessengerClient::disconnect() {
}

bool TCPMessengerClient::getAllUsers() {
	if (!this->isConnected() || this->status == AUTH) {
		return false;
	}

	ServerIO::sendCommandToPeer(this->socket, PRINT_USERS_REQ);
	return true;
}

bool getConnectedUsers() {
}

bool TCPMessengerClient::getAllRooms() {
}

bool TCPMessengerClient::getRoomsUsers(string room) {
}

bool TCPMessengerClient::login(string name, string password) {
	if (this->status != AUTH) {
		cout << "You can't log in" << endl;

		return false;
	}

	ServerIO::sendCommandToPeer(this->socket, LOGIN_REQ);
	ServerIO::sendDataToPeer(this->socket, name + " " + password);

	return true;
}

bool TCPMessengerClient::registerUser(string name, string password) {
	if (this->status != AUTH) {
		cout << "You can't register" << endl;

		return false;
	}

	ServerIO::sendCommandToPeer(this->socket, REGISTER_REQ);
	ServerIO::sendDataToPeer(this->socket, name + " " + password);

	return true;
}

string TCPMessengerClient::getServerIp() {
}
string TCPMessengerClient::getPeerName() {
}
string TCPMessengerClient::getPeerIpAndPort() {
}
string TCPMessengerClient::getConversation() {
	return this->currentConversation;
}
bool TCPMessengerClient::openSession(string user) {
}
bool TCPMessengerClient::joinRoom(string room) {
}
bool TCPMessengerClient::closeRoom(string room) {
}
bool TCPMessengerClient::exitRoom() {
}
bool TCPMessengerClient::isActiveBroker() {
	return this->status == BROKER;
}
bool TCPMessengerClient::isActiveRoom() {
	return this->status == ROOM;
}
bool TCPMessengerClient::closeActiveSession() {
}
bool TCPMessengerClient::send(string msg) {
}

void TCPMessengerClient::run() {
	this->isRunning = true;

	while (this->isRunning) {
		int command = ServerIO::readCommandFromPeer(this->socket);

		switch (command) {
		case (LOGIN_FAILED): {
			cout << "login failed" << endl;
			break;
		}
		case (USER_ALREADY_EXISTS_RES): {
			cout << "user already exists" << endl;
			break;
		}
		case (PORT_INIT_REQ): {
			ServerIO::sendCommandToPeer(this->socket, PORT_INIT_RES);
			ServerIO::sendDataToPeer(this->socket,
					to_string(this->udpMessenger->getPort()));
			break;
		}
		case (ALREADY_CONNECTED_RES): {
			cout << "user already connected" << endl;

			break;
		}
		case (IN_DISPATCHER): {
			this->status = DISPATCHER;
			break;
		}
		case (SUCCESSFULY_LOGIN_RES): {
			this->user = ServerIO::readDataFromPeer(this->socket);
			cout << "Welcome " << this->user << endl;

			break;
		}
		case (PRINT_USERS_RES): {
			string users = ServerIO::readDataFromPeer(this->socket);
			cout << "Users: " << users << endl;
			break;
		}
		default: {
			break;
		}
		}
	}
}

string TCPMessengerClient::getStatus() {
	switch (this->status) {
	case (DISCONNECTED): {
		return "disconnected";
	}
	case (AUTH): {
		return "waiting for credentials";
	}
	case (DISPATCHER): {
		return "connected to server " + this->getServerIp();
	}
	case (BROKER): {
		return "chatting with " + this->getConversation();
	}
	case (ROOM): {
		return "in room " + this->getConversation();
	}
	}

	return "unknown status";
}

void TCPMessengerClient::handleMessage(string msg) {

// TODO:
	cout << msg << endl;
}

