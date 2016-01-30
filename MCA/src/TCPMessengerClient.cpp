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

	cout << this->udpMessenger->getPort() << endl;

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
}

bool TCPMessengerClient::registerUser(string name, string password) {
}

string TCPMessengerClient::getServerIp() {
}
string TCPMessengerClient::getPeerName() {
}
string TCPMessengerClient::getPeerIpAndPort() {
}
string TCPMessengerClient::getRoomName() {
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
}
bool TCPMessengerClient::isActiveRoom() {
}
bool TCPMessengerClient::closeActiveSession() {
}
bool TCPMessengerClient::send(string msg) {
}

void TCPMessengerClient::run() {
}

void TCPMessengerClient::handleMessage(string msg) {

	// TODO:
	cout << msg << endl;
}

