/*
 * TCPMessengerServer.h
 *
 * Listens to the port and moves peers to their dispatchers.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

/***********************   TCPMessengerServer implementation ******************************/

/**
	 * Build the Messenger server
	 */
TCPMessengerServer::TCPMessengerServer() {
	this->serverSocket = new TCPSocket(MSNGR_PORT);
	this->dispatcher = new TCPMessengerDispatcher();
	this->authDispatcher = new AuthDispatcher(this);
	this->start();
}

/**
	 * Close the server and disconnect all the users
	 */
void TCPMessengerServer::close() {
	this->dispatcher->shutdown();
	this->authDispatcher->shutdown();
	this->serverSocket->cclose();
}

TCPMessengerServer::~TCPMessengerServer() {
	delete this->dispatcher;
	delete this->authDispatcher;
	delete this->serverSocket;
}

/*
 * Print out the list of connected clients (for debug)
 */
void TCPMessengerServer::listPeers() {
	this->dispatcher->printKeys();
}

/*
 * Listen and add accept peers
 */
void TCPMessengerServer::run() {
	while (this->serverSocket != NULL) {
		TCPSocket* listener = this->serverSocket->listenAndAccept();
		this->authDispatcher->addSocket(listener);
	}
}
/**
 * Handle user authenticated
 */
void TCPMessengerServer::userLogin(TCPSocket* peer, user_name name) {
	ServerIO::sendCommandToPeer(peer, PORT_INIT_REQ);

	int command = ServerIO::readCommandFromPeer(peer);

	// Must send port after the login
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

	this->dispatcher->addSocket(peer, name, ServerIO::readDataFromPeer(peer));
}

/*
 * Tells whether a user is connected
 */
bool TCPMessengerServer::isUserConnected(user_name name) {
	return this->dispatcher->isUserConnected(name);
}

/**
 * A string representation of all the connected users
 */
string TCPMessengerServer::getConnectUsers() {
	string users = this->dispatcher->getConnectUsers();

	if (users.empty()) {
		return "There is no connected users.";
	}

	return "Connected users: " + users;
}

/**
 * A string representation of all the brokers
 */
string TCPMessengerServer::getAllBrokers() {
	string brokers = this->dispatcher->getAllBrokers();

	if (brokers.empty()) {
		return "There is no sessions.";
	}

	return "Sessions: " + brokers;
}

/**
 * A string representation of all the rooms
 */
string TCPMessengerServer::getRoomsNames() {
	string rooms = this->dispatcher->getRoomsNames();

	if (rooms.empty()) {
		return "There is no rooms";
	}

	return "Rooms: " + rooms;
}

/**
 * A string representation of all the room users
 */
string TCPMessengerServer::getRoomsUsers(room_name roomName){
	Room* room = dispatcher->roomExists(roomName);

	if (room) {
		return "Room " + roomName + " users: " + room->getRoomsUsers();
	}

	return "Room " + roomName + " does not exist.";
}
