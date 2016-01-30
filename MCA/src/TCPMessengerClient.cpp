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
	this->currentRoom = "";
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
string TCPMessengerClient::getRoomName() {
	return this->currentRoom;
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
		default: {
			break;
		}
		}
	}
}

int TCPMessengerClient::getStatus() {
	return this->status;
}

void TCPMessengerClient::handleMessage(string msg) {

// TODO:
	cout << msg << endl;
}

