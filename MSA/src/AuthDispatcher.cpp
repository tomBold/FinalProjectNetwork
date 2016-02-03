/*
 * AuthDispatcher.h
 *
 * This class is responsible of handling all the unauthenticated users
 * by letting them authenticate or register.
 *
 *  Created on: Jan 29, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "AuthDispatcher.h"

AuthDispatcher::AuthDispatcher(TCPMessengerServer* tcpMessengerServer) {
	this->tcpMessengerServer = tcpMessengerServer;
	this->start();
}

AuthDispatcher::~AuthDispatcher() {
}

/*
 * Add socket to the map and the multiple tcp socket listener
 */
void AuthDispatcher::addSocket(TCPSocket* socket) {
	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);
}

/*
 * Delete socket by ip and port
 */
void AuthDispatcher::deleteSocket(ip_and_port ipAndPort) {
	map<string, TCPSocket*>::iterator it = this->sockets.find(ipAndPort);
	this->multiSocketListener->removeSocket(it->second);
	this->sockets.erase(it);
}

/*
 * Check if the socket exists in the sockets map
 */
bool AuthDispatcher::isSocketExists(TCPSocket* socket) {
	return this->sockets.find(socket->destIpAndPort()) != this->sockets.end();
}

/*
 * Handle socket - gets the socket's command and executes it
 */
void AuthDispatcher::handleSocket(TCPSocket* socket) {
	if (!this->isSocketExists(socket)) {
		return;
	}

	int command = ServerIO::readCommandFromPeer(socket);

	switch (command) {
	case (LOGIN_REQ): {
		string name;
		string password;

		this->getUserAndPasswordFromSocket(socket, &name, &password);
		if (this->tcpMessengerServer->isUserConnected(name)) {
			ServerIO::sendCommandToPeer(socket, ALREADY_CONNECTED_RES);
		} else if (!Users::login(name, password)) {
			ServerIO::sendCommandToPeer(socket, LOGIN_FAILED);
		} else {
			this->userLogin(socket, name);
		}

		break;
	}
	case (REGISTER_REQ): {
		string name;
		string password;

		this->getUserAndPasswordFromSocket(socket, &name, &password);

		if (Users::create(name, password)) {
			cout << name << " register successfully" << endl;
			this->userLogin(socket, name);
		} else {
			cout << name << " register failed" << endl;

			if (Users::contains(name)) {
				ServerIO::sendCommandToPeer(socket,
				USER_ALREADY_EXISTS_RES);
			}
		}

		break;
	}
	case (0): {
		this->disconnectClient(socket);
		break;
	}
	case (DISCONNECT_FROM_SERVER_REQ): {
		this->disconnectClient(socket);
		break;
	}
	default: {
		cout << "Command " << command << " not found" << endl;
		break;
	}
	}
}

/*
 * Handle exit socket
 */
void AuthDispatcher::disconnectClient(TCPSocket* socket) {
	this->deleteSocket(socket);
	socket->cclose();
	delete socket;
}

/**
 * Authenticate user
 */
void AuthDispatcher::userLogin(TCPSocket* socket, user_name name) {
	this->deleteSocket(socket);
	this->tcpMessengerServer->userLogin(socket, name);
}

/**
 * Read user and password from socket and put them in the
 * respective out parameters.
 */
void AuthDispatcher::getUserAndPasswordFromSocket(TCPSocket* socket,
		user_name* name, string* password) {
	string userNameAndPassword = ServerIO::readDataFromPeer(socket);

	int index = userNameAndPassword.find(" ");

	(*name) = userNameAndPassword.substr(0, index);
	(*password) = userNameAndPassword.substr(index + 1);
}

/**
 * Shutdown and disconnect every user
 */
void AuthDispatcher::shutdown() {
	for (map<string, TCPSocket*>::const_iterator it = this->sockets.begin();
			it != this->sockets.end(); it++) {
		ServerIO::sendCommandToPeer(it->second, DISCONNECT_CLIENTS);
		this->disconnectClient(it->second);
	}

	this->isRunning = false;
}
