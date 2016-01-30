/*
 * TCPMessengerDispatcher.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#include "TCPMessengerDispatcher.h"

TCPMessengerDispatcher::TCPMessengerDispatcher() {
	this->isRunning = true;
	this->multiSocketListener = new ExtendedMultipleTCPSocketListener();
	this->start();
}

TCPMessengerDispatcher::~TCPMessengerDispatcher() {
	delete this->multiSocketListener;

	// Delete the map sockets
	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		delete it->second;
	}
}

/*
 * Add socket to the map and the multiple tcp socket listener
 */
void TCPMessengerDispatcher::addSocket(TCPSocket* socket, string name,
		string port) {
	this->peersIpToUser[socket->destIpAndPort()] = name;
	this->userToPeersIp[name] = socket->destIpAndPort();
	this->peersIpToPort[socket->destIpAndPort()] = port;
	this->addSocket(socket);
}

/*
 * Add socket to the map and the multiple tcp socket listener
 */
void TCPMessengerDispatcher::addSocket(TCPSocket* socket) {
	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);

	ServerIO::sendCommandToPeer(socket, IN_DISPATCHER);
}

/*
 * Delete socket by socket
 */
void TCPMessengerDispatcher::deleteSocket(TCPSocket* socket) {
	this->deleteSocket(socket->destIpAndPort());
}

/*
 * Delete socket by ip and port
 */
void TCPMessengerDispatcher::deleteSocket(string socketKey) {
	this->multiSocketListener->removeSocket(this->sockets[socketKey]);
	this->sockets.erase(this->sockets.find(socketKey));
}

/*
 * Print the sockets keys
 */
void TCPMessengerDispatcher::printKeys() {
	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		string key = it->first;
		cout << key << endl;
	}
}

/**
 * Retrieve commands from peers
 */
void TCPMessengerDispatcher::run() {
	while (this->isRunning) {
		TCPSocket* currSocket = this->multiSocketListener->listenToSocket();

		if (currSocket != NULL) {
			this->handleSocket(currSocket);
		}
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
 * Check if the socket exists in the sockets map
 */
bool TCPMessengerDispatcher::isSocketExists(TCPSocket* socket) {
	return this->sockets.find(socket->destIpAndPort()) != this->sockets.end();
}

/*
 * Handle socket - gets the socket's command and executes it
 */
void TCPMessengerDispatcher::handleSocket(TCPSocket* socket) {
	if (!this->isSocketExists(socket)) {
		return;
	}

	this->handleSocketCommand(socket, ServerIO::readCommandFromPeer(socket));
}

void TCPMessengerDispatcher::handleSocketCommand(TCPSocket* socket,
		int command) {
	string name = this->peersIpToUser[socket->destIpAndPort()];

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
	default: {
		cout << "Command " << command << " not found" << endl;
		break;
	}
	}
}

/*
 * Get sockets vector
 */
vector<TCPSocket*> TCPMessengerDispatcher::getSockets() {
	vector<TCPSocket*> socketsVec;

	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		socketsVec.push_back(it->second);
	}

	return socketsVec;
}

/*
 * Create session
 */
void TCPMessengerDispatcher::createSession(TCPSocket* socket, string peerName) {
	if (!isUserConnected(peerName)) {
		ServerIO::sendCommandToPeer(socket, SESSION_REFUSED);
		return;
	}

	string peerIP = this->userToPeersIp[peerName];

	// If the socket try to connect to itself
	if (socket->destIpAndPort() == peerIP) {
		ServerIO::sendCommandToPeer(socket, SESSION_REFUSED);
	} else {
		TCPSocket* peerSocket = this->sockets[peerIP];
		this->createBroker(socket, peerSocket);
	}
}

/*
 * Handle exit socket
 */
void TCPMessengerDispatcher::disconnectClient(TCPSocket* socket) {
	string ip = socket->destIpAndPort();
	string name = this->peersIpToUser[ip];

	this->deleteSocket(socket);
	socket->cclose();
	delete socket;

	this->userToPeersIp.erase(name);
	this->peersIpToPort.erase(ip);
	this->peersIpToUser.erase(ip);
}

void TCPMessengerDispatcher::closeBroker(Broker* broker) {
	// Adding back
	this->addSocket(broker->firstSocket);
	this->addSocket(broker->secondSocket);

	ServerIO::sendCommandToPeer(broker->firstSocket,
	CLOSE_SESSION_WITH_PEER);
	ServerIO::sendCommandToPeer(broker->secondSocket,
	CLOSE_SESSION_WITH_PEER);

	this->brokers.erase(broker);
	delete broker;
}

bool TCPMessengerDispatcher::createRoom(string roomName, TCPSocket* admin) {
	if (this->roomExists(roomName)) {
		return false;
	}

	Room* room = new Room(roomName, this->peersIpToUser[admin->destIpAndPort()],
			admin, this);

	this->rooms.insert(room);
	this->deleteSocket(admin);
	room->start();

	return true;
}

void TCPMessengerDispatcher::closeRoom(Room* room) {
	map<string, TCPSocket*> users = room->users;

	for (map<string, TCPSocket*>::const_iterator it = users.begin();
			it != users.end(); it++) {
		this->addSocket(it->second);
	}

	this->rooms.erase(room);
	delete room;
}

void TCPMessengerDispatcher::leaveRoom(TCPSocket* socket) {
	this->addSocket(socket);
}

bool TCPMessengerDispatcher::joinRoom(string roomName, TCPSocket* socket) {
	Room* room = this->roomExists(roomName);

	if (!room) {
		return false;
	}

	if (!room->join(this->peersIpToUser[socket->destIpAndPort()], socket)) {
		return false;
	}

	this->deleteSocket(socket);
	ServerIO::sendCommandToPeer(socket, SUCCESSFULY_JOIN_ROOM);
	ServerIO::sendDataToPeer(socket, roomName);

	return true;
}

Room* TCPMessengerDispatcher::roomExists(string roomName) {
	set<Room*>::iterator it;
	for (it = this->rooms.begin(); it != this->rooms.end(); it++) {
		Room* room = *it;

		if (room->name == roomName) {
			return room;
		}
	}

	return NULL;
}

bool TCPMessengerDispatcher::isUserBusy(string name) {
	set<Room*>::iterator it;
	for (it = this->rooms.begin(); it != this->rooms.end(); it++) {
		Room* room = *it;

		if (room->exists(name)) {
			return true;
		}
	}

	set<Broker*>::iterator itB;
	string ip = this->userToPeersIp[name];
	for (itB = this->brokers.begin(); itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;

		if (broker->isUserInBroker(ip)) {
			return true;
		}
	}

	return false;
}

void TCPMessengerDispatcher::forceLeaveUser(string name) {
	set<Room*>::iterator it;
	for (it = this->rooms.begin(); it != this->rooms.end(); it++) {
		Room* room = *it;

		if (room->exists(name)) {
			room->leave(name);
			return;
		}
	}

	set<Broker*>::iterator itB;
	string ip = this->userToPeersIp[name];
	for (itB = this->brokers.begin(); itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;

		if (broker->isUserInBroker(ip)) {
			this->closeBroker(broker);
			return;
		}
	}
}

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

string TCPMessengerDispatcher::getConnectUsers() {
	string result = "";
	for (map<string, string>::const_iterator it = this->userToPeersIp.begin();
			it != this->userToPeersIp.end(); it++) {
		if (result != "") {
			result += ",";
		}
		result += it->first;
	}

	return result;
}

string TCPMessengerDispatcher::getAllBrokers() {
	string result = "";

	set<Broker*>::iterator itB;
	for (itB = this->brokers.begin(); itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;

		if (result != "") {
			result += "\n";
		}

		result += this->peersIpToUser[broker->firstSocket->destIpAndPort()]
				+ "<>"
				+ this->peersIpToUser[broker->secondSocket->destIpAndPort()];

	}

	return result;
}

void TCPMessengerDispatcher::shutdown() {
	for (set<Room*>::iterator itR = this->rooms.begin();
			itR != this->rooms.end(); itR++) {
		Room* room = *itR;
		room->close(room->admin);
	}

	for (set<Broker*>::iterator itB = this->brokers.begin();
			itB != this->brokers.end(); itB++) {
		Broker* broker = *itB;
		this->closeBroker(broker);
	}

	for (map<string, TCPSocket*>::const_iterator it = this->sockets.begin();
			it != this->sockets.end(); it++) {
		this->disconnectClient(it->second);
	}

	this->isRunning = false;
}

string TCPMessengerDispatcher::getUserP2PAddress(TCPSocket* socket) {
	return socket->fromAddr() + ":"
			+ this->peersIpToPort[socket->destIpAndPort()] + ":"
			+ this->peersIpToUser[socket->destIpAndPort()];
}

bool TCPMessengerDispatcher::isUserConnected(string name) {
	return this->userToPeersIp.find(name) != this->userToPeersIp.end();
}

