/*
 * TCPMessengerDispatcher.cpp
 *
 * Manages login user requests.
 *
 *  Created on: Feb 3, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "TCPMessengerDispatcher.h"

TCPMessengerDispatcher::TCPMessengerDispatcher() {
	// Initialize properties
	this->isRunning = true;
	this->multiSocketListener = new ExtendedMultipleTCPSocketListener();
	this->start();
}

TCPMessengerDispatcher::~TCPMessengerDispatcher() {
	delete this->multiSocketListener;

	// Delete the map sockets
	for (map<string, TCPSocket*>::const_iterator it = this->sockets.begin();
			it != this->sockets.end(); it++) {
		delete it->second;
	}
}

/*
 * Add socket to the map and the multiple tcp socket listener.
 */
void TCPMessengerDispatcher::addSocket(TCPSocket* socket, user_name name,
		port port) {
	// Initialize maps
	this->peerIpToUser[socket->destIpAndPort()] = name;
	this->userToPeerIp[name] = socket->destIpAndPort();
	this->peerIpToPort[socket->destIpAndPort()] = port;

	this->addSocket(socket);
}

/*
 * Add socket to the map and the multiple tcp socket listener.
 */
void TCPMessengerDispatcher::addSocket(TCPSocket* socket) {
	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);

	ServerIO::sendCommandToPeer(socket, IN_DISPATCHER);
}

/*
 * Delete socket by ip and port
 */
void TCPMessengerDispatcher::deleteSocket(ip_and_port ipAndPort) {
	this->multiSocketListener->removeSocket(this->sockets[ipAndPort]);
	this->sockets.erase(this->sockets.find(ipAndPort));
}

/*
 * Print the sockets keys
 */
void TCPMessengerDispatcher::printKeys() {
	for (map<string, TCPSocket*>::const_iterator it = this->sockets.begin();
			it != this->sockets.end(); it++) {
		string key = it->first;
		cout << key << endl;
	}
}

/*
 * Create a broker
 */
void TCPMessengerDispatcher::createBroker(TCPSocket* firstSocket,
		TCPSocket* secondSocket) {
	Broker* broker = new Broker(firstSocket, secondSocket, this);
	this->brokers.insert(broker);

	// Delete sockets
	this->deleteSocket(firstSocket);
	this->deleteSocket(secondSocket);

	broker->start();
}

/*
 * Check if the socket exists in the sockets map.
 */
bool TCPMessengerDispatcher::isSocketExists(TCPSocket* socket) {
	return this->sockets.find(socket->destIpAndPort()) != this->sockets.end();
}

/*
 * Handle socket - gets the socket's command and executes it.
 */
void TCPMessengerDispatcher::handleSocket(TCPSocket* socket) {
	if (!this->isSocketExists(socket)) {
		return;
	}

	this->handleSocketCommand(socket, ServerIO::readCommandFromPeer(socket));
}

/*
 * Gets the socket's command and executes it.
 */
void TCPMessengerDispatcher::handleSocketCommand(TCPSocket* socket,
		int command) {
	string name = this->getUserByPeerIp(socket->destIpAndPort());

	switch (command) {
	case (CLOSE_SESSION_WITH_PEER): {
		this->forceLeaveUser(name);
		break;
	}
	case (OPEN_SESSION_WITH_PEER): {
		this->forceLeaveUser(name);
		string peerName = ServerIO::readDataFromPeer(socket);
		createSession(socket, peerName);

		break;
	}
	case (0): {
		this->forceLeaveUser(name);
		this->disconnectClient(socket);

		break;
	}
	case (DISCONNECT_FROM_SERVER_REQ): {
		this->forceLeaveUser(name);
		this->disconnectClient(socket);

		break;
	}
	case (JOIN_ROOM_REQ): {
		string room = ServerIO::readDataFromPeer(socket);
		this->forceLeaveUser(name);
		this->joinRoom(room, socket);

		break;
	}
	case (PRINT_ROOMS_USERS_REQ): {
		string roomName = ServerIO::readDataFromPeer(socket);
		Room* room = this->roomExists(roomName);

		if (room != NULL) {
			string roomUsers = room->getRoomsUsers();
			ServerIO::sendCommandToPeer(socket,
			PRINT_ROOMS_USERS_RES);
			ServerIO::sendDataToPeer(socket, roomUsers);
		} else {
			ServerIO::sendCommandToPeer(socket, ROOM_NOT_EXISTS);
			ServerIO::sendDataToPeer(socket, roomName);
		}

		break;
	}
	case (PRIMT_ROOMS_NAMES_REQ): {
		string roomsName = this->getRoomsNames();
		ServerIO::sendCommandToPeer(socket,
		PRIMT_ROOMS_NAMES_RES);
		ServerIO::sendDataToPeer(socket, roomsName);

		break;
	}
	case (PRINT_CONNECT_USERS_REQ): {
		string users = this->getConnectUsers();
		ServerIO::sendCommandToPeer(socket,
		PRINT_CONNECT_USERS_RES);
		ServerIO::sendDataToPeer(socket, users);

		break;
	}
	case (PRINT_USERS_REQ): {
		string users = Users::getAllUsers();
		ServerIO::sendCommandToPeer(socket, PRINT_USERS_RES);
		ServerIO::sendDataToPeer(socket, users);

		break;
	}
	default: {
		cout << "Command " << command << " not found." << endl;
		break;
	}
	}
}

/**
 * Create session between two users.
 */
void TCPMessengerDispatcher::createSession(TCPSocket* socket,
		user_name userName) {
	// Check if the wanted user to start communicate with is connected and if
	// the user is not in other session or room.
	if (!isUserConnected(userName) || this->isUserBusy(userName, false)) {
		ServerIO::sendCommandToPeer(socket, SESSION_REFUSED);
		return;
	}

	string peerIP = this->getPeerIpByUser(userName);

	// Check if the socket try to connect to itself.
	if (socket->destIpAndPort() == peerIP) {
		ServerIO::sendCommandToPeer(socket, SESSION_REFUSED);
	} else {
		TCPSocket* peerSocket = this->sockets[peerIP];
		this->createBroker(socket, peerSocket);
	}
}

/**
 * Disconnect a client.
 */
void TCPMessengerDispatcher::disconnectClient(TCPSocket* socket) {
	string ip = socket->destIpAndPort();
	string name = this->getUserByPeerIp(ip);

	this->deleteSocket(socket);
	socket->cclose();
	delete socket;

	this->userToPeerIp.erase(name);
	this->peerIpToPort.erase(ip);
	this->peerIpToUser.erase(ip);
}

/**
 * Close broker.
 */
void TCPMessengerDispatcher::closeBroker(Broker* broker) {
	broker->areBothPeersConnected = false;

	// Adding the user back to the free connected users map.
	this->addSocket(broker->firstSocket);
	this->addSocket(broker->secondSocket);

	ServerIO::sendCommandToPeer(broker->firstSocket,
	CLOSE_SESSION_WITH_PEER);
	ServerIO::sendCommandToPeer(broker->secondSocket,
	CLOSE_SESSION_WITH_PEER);
}

/**
 * Create a new room.
 */
bool TCPMessengerDispatcher::createRoom(room_name roomName, TCPSocket* admin) {
	// Check if the room name is already exists.
	if (this->roomExists(roomName)) {
		return false;
	}

	// Creates the new room.
	Room* room = new Room(roomName,
			this->getUserByPeerIp(admin->destIpAndPort()), admin, this);
	this->rooms.insert(room);
	this->deleteSocket(admin);
	room->start();

	return true;
}

/**
 * Close room.
 */
void TCPMessengerDispatcher::closeRoom(Room* room) {
	room->roomIsOpen = false;

	map<string, TCPSocket*> users = room->users;

	// Add back all the rooms' users to the free connected users map.
	for (map<string, TCPSocket*>::const_iterator it = users.begin();
			it != users.end(); it++) {
		this->addSocket(it->second);
	}
}

/**
 * Leave room.
 */
void TCPMessengerDispatcher::leaveRoom(TCPSocket* socket) {
	this->addSocket(socket);
}

/**
 * Join room.
 */
bool TCPMessengerDispatcher::joinRoom(room_name roomName, TCPSocket* socket) {
	Room* room = this->roomExists(roomName);

	// Check if the room exists.
	if (!room) {
		return this->createRoom(roomName, socket);
	}

	// Try to join the room.
	if (!room->join(this->getUserByPeerIp(socket->destIpAndPort()), socket)) {
		return false;
	}

	this->deleteSocket(socket);
	ServerIO::sendCommandToPeer(socket, SUCCESSFULLY_JOIN_ROOM);
	ServerIO::sendDataToPeer(socket, roomName);

	return true;
}

/**
 * Get the room if the room exists, otherwise return NULL.
 */
Room* TCPMessengerDispatcher::roomExists(room_name roomName) {
	set<Room*>::iterator it;
	for (it = this->rooms.begin(); it != this->rooms.end(); it++) {
		Room* room = *it;

		if (room->name == roomName) {
			return room;
		}
	}

	return NULL;
}

/**
 * return if the user is in active room or broker.
 */
bool TCPMessengerDispatcher::isUserBusy(user_name name, bool forceLeaveUser) {
	// Check if the user is in active room.
	set<Room*>::iterator it;
	for (it = this->rooms.begin(); it != this->rooms.end(); it++) {
		Room* room = *it;

		if (room->exists(name)) {
			if (forceLeaveUser) {
				room->leave(name);
			}

			return true;
		}
	}

	// Check if the user is in active broker.
	set<Broker*>::iterator itB;
	string ip = this->userToPeerIp[name];
	for (itB = this->brokers.begin(); itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;

		if (broker->isUserInBroker(ip)) {
			if (forceLeaveUser) {
				this->closeBroker(broker);
			}

			return true;
		}
	}

	return false;
}

/**
 * Force user to leave his active communication (room or broker).
 */
void TCPMessengerDispatcher::forceLeaveUser(user_name name) {
	this->isUserBusy(name, true);
}

/**
 * Get the rooms' names.
 */
string TCPMessengerDispatcher::getRoomsNames() {
	string roomsNames = "";
	set<Room*>::iterator it;

	for (it = this->rooms.begin(); it != this->rooms.end(); it++) {
		Room* room = *it;
		if (roomsNames != "") {
			roomsNames += ",";
		}

		roomsNames += room->name;
	}

	return roomsNames;
}

/**
 * Gets the connected users.
 */
string TCPMessengerDispatcher::getConnectUsers() {
	string result = "";

	for (map<string, string>::const_iterator it = this->userToPeerIp.begin();
			it != this->userToPeerIp.end(); it++) {
		if (result != "") {
			result += ",";
		}

		result += it->first;
	}

	return result;
}

/**
 * Gets the brockers.
 */
string TCPMessengerDispatcher::getAllBrokers() {
	string result = "";

	set<Broker*>::iterator itB;
	for (itB = this->brokers.begin(); itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;

		if (result != "") {
			result += ",";
		}

		// Building the description conversation.
		result += this->peerIpToUser[broker->firstSocket->destIpAndPort()]
				+ "<>"
				+ this->peerIpToUser[broker->secondSocket->destIpAndPort()];
	}

	return result;
}

/**
 * Shutdown
 */
void TCPMessengerDispatcher::shutdown() {
	// Close the rooms.
	for (set<Room*>::iterator itR = this->rooms.begin();
			itR != this->rooms.end(); itR++) {
		Room* room = *itR;
		room->close(room->admin);
	}

	// CLose the brokers.
	for (set<Broker*>::iterator itB = this->brokers.begin();
			itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;
		this->closeBroker(broker);
	}

	// Disconnect the clients.
	for (map<string, TCPSocket*>::const_iterator it = this->sockets.begin();
			it != this->sockets.end(); it++) {
		ServerIO::sendCommandToPeer(it->second, DISCONNECT_CLIENTS);
		this->disconnectClient(it->second);
	}

	this->isRunning = false;
}

/**
 * Gets the user peer to peer address.
 */
string TCPMessengerDispatcher::getUserP2PAddress(TCPSocket* socket) {
	return socket->fromAddr() + ":"
			+ this->peerIpToPort[socket->destIpAndPort()] + ":"
			+ this->peerIpToUser[socket->destIpAndPort()];
}

/**
 * Return if the user connected.
 */
bool TCPMessengerDispatcher::isUserConnected(user_name name) {
	return this->userToPeerIp.find(name) != this->userToPeerIp.end();
}

/**
 * Delete room.
 */
void TCPMessengerDispatcher::deleteRoom(Room* room) {
	this->rooms.erase(room);
	delete room;
}

/**
 * Delete broker.
 */
void TCPMessengerDispatcher::deleteBroker(Broker* broker) {
	this->brokers.erase(broker);
	delete broker;
}

/**
 * Gets user by peer ip and port.
 */
string TCPMessengerDispatcher::getUserByPeerIp(ip_and_port peerIp) {
	return this->peerIpToUser[peerIp];
}

/**
 * Gets peer ip and port by user.
 */
string TCPMessengerDispatcher::getPeerIpByUser(user_name user) {
	return this->userToPeerIp[user];
}

/**
 * Gets port by peer ip and port.
 */
string TCPMessengerDispatcher::getPortByPeerIp(ip_and_port peerIp) {
	return this->peerIpToPort[peerIp];
}
