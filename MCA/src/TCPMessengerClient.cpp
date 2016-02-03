/*
 * TCPMessengerClient.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "TCPMessengerClient.h"
using namespace std;
TCPMessengerClient::TCPMessengerClient() {
	this->socket = NULL;
	setStatus(DISCONNECTED);
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

	setStatus(DISCONNECTED);
}

bool TCPMessengerClient::connect(string ip) {
	if (this->isConnected()) {
		cout << "Already connected to a server. press d to disconnect." << endl;
		return false;
	}

	try {
		this->socket = new TCPSocket(ip, MSNGR_PORT);
		this->udpMessenger = new UDPMessenger();
		setStatus(AUTH);
		this->start();

		return true;
	} catch (const char* e) {
		return false;
	}
}

bool TCPMessengerClient::isConnected() {
	return this->status != DISCONNECTED;
}

bool TCPMessengerClient::disconnect() {
	if (this->isConnected()) {
		ServerIO::sendCommandToPeer(this->socket, DISCONNECT_FROM_SERVER_REQ);
		setStatus(DISCONNECTED);
		this->currentConversation = "";
		this->isRunning = false;
		this->udpMessenger->close();
		this->user = "";
		this->socket->cclose();
		delete this->socket;
		delete this->udpMessenger;

		this->udpMessenger = NULL;
		this->socket = NULL;

		return true;
	}

	return false;
}

bool TCPMessengerClient::getAllUsers() {
	return this->sendCommand(PRINT_USERS_REQ);
}

bool TCPMessengerClient::getConnectedUsers() {
	return this->sendCommand(PRINT_CONNECT_USERS_REQ);
}

bool TCPMessengerClient::isLoggedIn() {
	return this->isConnected() && this->status != AUTH;
}

bool TCPMessengerClient::getAllRooms() {
	return this->sendCommand(PRIMT_ROOMS_NAMES_REQ);
}

bool TCPMessengerClient::getRoomsUsers(string room) {
	if (this->sendCommand(PRINT_ROOMS_USERS_REQ)) {
		ServerIO::sendDataToPeer(this->socket, room);

		return true;
	}

	return false;
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
	return this->socket->destIpAndPort();
}

string TCPMessengerClient::getConversation() {
	return this->currentConversation;
}
bool TCPMessengerClient::openSession(string user) {
	if (this->sendCommand(OPEN_SESSION_WITH_PEER)) {
		ServerIO::sendDataToPeer(this->socket, user);
		return true;
	}

	return false;
}

bool TCPMessengerClient::joinRoom(string room) {
	if (sendCommand(JOIN_ROOM_REQ)) {
		ServerIO::sendDataToPeer(this->socket, room);
	}

	return false;
}

bool TCPMessengerClient::closeRoom(string room) {
	return sendCommand(CLOSE_ROOM_REQ);
}

bool TCPMessengerClient::exitRoom() {
	return this->closeActiveSession();
}

bool TCPMessengerClient::isActiveBroker() {
	return this->status == BROKER;
}

bool TCPMessengerClient::isActiveRoom() {
	return this->status == ROOM;
}

bool TCPMessengerClient::closeActiveSession() {
	if (this->isActiveBroker() || this->isActiveRoom()) {
		ServerIO::sendCommandToPeer(this->socket, CLOSE_SESSION_WITH_PEER);
		return true;
	}

	cout << "There is no active session" << endl;
	return false;
}
bool TCPMessengerClient::send(string msg) {
	if (this->isActiveRoom() || this->isActiveBroker()) {
		this->udpMessenger->send(msg);
		return true;
	}

	cout << "There is no active session" << endl;
	return false;
}

void TCPMessengerClient::run() {
	this->isRunning = true;

	while (this->isRunning) {
		int command = ServerIO::readCommandFromPeer(this->socket);

		switch (command) {
		case (LOGIN_FAILED): {
			cout << "Login failed" << endl;
			break;
		}
		case (USER_ALREADY_EXISTS_RES): {
			cout << "User already exists" << endl;
			break;
		}
		case (PORT_INIT_REQ): {
			ServerIO::sendCommandToPeer(this->socket, PORT_INIT_RES);
			ServerIO::sendDataToPeer(this->socket, to_string(this->udpMessenger->getPort()));
			break;
		}
		case (ALREADY_CONNECTED_RES): {
			cout << "User already connected" << endl;

			break;
		}
		case (IN_DISPATCHER): {
			setStatus(DISPATCHER);
			this->currentConversation = "";

			break;
		}
		case (SUCCESSFULLY_LOGIN_RES): {
			this->user = ServerIO::readDataFromPeer(this->socket);
			cout << "Welcome " << this->user << endl;

			break;
		}
		case (PRINT_USERS_RES): {
			string users = ServerIO::readDataFromPeer(this->socket);
			cout << "Users: " << users << endl;
			break;
		}
		case (PRINT_CONNECT_USERS_RES): {
			string users = ServerIO::readDataFromPeer(this->socket);
			cout << "Connected users: " << users << endl;
			break;
		}
		case (PRIMT_ROOMS_NAMES_RES): {
			string rooms = ServerIO::readDataFromPeer(this->socket);
			cout << "Rooms names: " << rooms << endl;
			break;
		}
		case (PRINT_ROOMS_USERS_RES): {
			string users = ServerIO::readDataFromPeer(this->socket);
			cout << "Rooms users: " << users << endl;
			break;
		}
		case (ROOM_NOT_EXISTS): {
			string room = ServerIO::readDataFromPeer(this->socket);
			cout << "The room " << room << " does not exist" << endl;
			break;
		}
		case (SESSION_REFUSED): {
			cout << "Session refused" << endl;
			break;
		}
		case (SESSION_ESTABLISHED): {
			this->currentConversation = ServerIO::readDataFromPeer(
					this->socket);
			setStatus(BROKER);
			break;
		}
		case (NEW_MESSAGE_DST_RES): {
			string dst = ServerIO::readDataFromPeer(this->socket);
			this->udpMessenger->setTheMsgDestination(dst);

			break;
		}
		case (IN_EMPTY_ROOM): {
			this->currentConversation = ServerIO::readDataFromPeer(
					this->socket);
			setStatus(ROOM);
			cout << "You are in empty room " << this->currentConversation
					<< endl;
			break;
		}
		case (USER_LEAVE_ROOM_RES): {
			string user = ServerIO::readDataFromPeer(this->socket);

			cout << user << " just left the room " << this->currentConversation
					<< endl;
			break;
		}
		case (SUCCESSFULLY_JOIN_ROOM): {
			this->currentConversation = ServerIO::readDataFromPeer(
					this->socket);
			setStatus(ROOM);
			break;
		}
		case (USER_JOIN_ROOM_RES): {
			string user = ServerIO::readDataFromPeer(this->socket);

			cout << user << " just join the room " << this->currentConversation
					<< endl;
			break;
		}
		case (CLOSE_ROOM_RES): {
			cout << "Close room " << this->currentConversation << endl;

			break;
		}
		case (DISCONNECT_CLIENTS): {
			setStatus(DISCONNECTED);
			this->currentConversation = "";
			this->isRunning = false;
			this->udpMessenger->close();
			this->user = "";
			this->socket->cclose();
			delete this->socket;
			delete this->udpMessenger;

			this->udpMessenger = NULL;
			this->socket = NULL;

			break;
		}
		default: {
			break;
		}
		}
	}
}

void TCPMessengerClient::printStatus() {
	string statusDescription = "unknown status";

	switch (this->status) {
	case (DISCONNECTED): {
		statusDescription = "disconnected";
		break;
	}
	case (AUTH): {
		statusDescription = "waiting for credentials";
		break;
	}
	case (DISPATCHER): {
		statusDescription = "connected to server " + this->getServerIp();
		break;
	}
	case (BROKER): {
		statusDescription = "chatting with " + this->getConversation();
		break;
	}
	case (ROOM): {
		statusDescription = "in room " + this->getConversation();
		break;
	}
	}

	cout << "Current status: " << statusDescription << endl;
}

bool TCPMessengerClient::sendCommand(int command) {
	if (!this->isLoggedIn()) {
		cout << "For this action you should log in" << endl;
		return false;
	}

	ServerIO::sendCommandToPeer(this->socket, command);
	return true;
}

void TCPMessengerClient::setStatus(int status) {
	if (this->status != status) {
		this->status = status;
		printStatus();
	}
}
