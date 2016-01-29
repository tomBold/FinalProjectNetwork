/*
 * TCPMessengerDispatcher.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#include "TCPMessengerDispatcher.h"

TCPMessengerDispatcher::TCPMessengerDispatcher() {
	this->multiSocketListener = new MultipleTCPSocketsListener();
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
void TCPMessengerDispatcher::addSocket(TCPSocket* socket, string name) {
	this->peersIpToUser[socket->destIpAndPort()] = name;
	this->userToPeersIp[name] = socket->destIpAndPort();
	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);
}

/*
 * Add socket to the map and the multiple tcp socket listener
 */
void TCPMessengerDispatcher::addSocket(TCPSocket* socket) {
	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);
}

/*
 * Delete socket by socket
 */
void TCPMessengerDispatcher::deleteSocket(TCPSocket* socket) {
	std::string destIpAndPort = socket->destIpAndPort();
	this->deleteSocket(destIpAndPort);
}

/*
 * Delete socket by key
 */
void TCPMessengerDispatcher::deleteSocket(string socketKey) {
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
 * Retrieve messages from peers
 */
void TCPMessengerDispatcher::run() {
	while (true) {
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
	Broker* b = new Broker(firstSocket, secondSocket, this);
	this->brokers.insert(b);

	// Delete sockets
	this->deleteSocket(firstSocket);
	this->deleteSocket(secondSocket);

	this->createMultipleTCPSocketListener();
	b->start();
}

/*
 * Check if the socket exists in the sockets map
 */
bool TCPMessengerDispatcher::isSocketExists(TCPSocket* socket) {
	TCPSocket* sock = this->sockets[socket->destIpAndPort()];

	if (sock == NULL) {
		this->deleteSocket(sock);

		return false;
	}

	return true;
}

/*
 * Handle socket - gets the socket's command and executes it
 */
void TCPMessengerDispatcher::handleSocket(TCPSocket* socket) {
	if (!this->isSocketExists(socket)) {
		return;
	}

	this->handleSocketCommand(socket,
			TCPMessengerServer::readCommandFromPeer(socket));
}

void TCPMessengerDispatcher::handleSocketCommand(TCPSocket* socket,
		int command) {
	switch (command) {
	case (CLOSE_SESSION_WITH_PEER): {
		cout << "Close session with peer" << endl;
		break;
	}
	case (OPEN_SESSION_WITH_PEER): {
		cout << "Open session with peer" << endl;
		string peer = TCPMessengerServer::readDataFromPeer(socket);
		createSession(socket, peer);
		break;
	}
	case (0): {
		this->exit(socket);
		break;
	}
	case (EXIT): {
		this->exit(socket);
		break;
	}
	case (SESSION_REFUSED): {
		cout << "Session refused" << endl;
		break;
	}
	case (SESSION_ESTABLISHED): {
		cout << "Session established" << endl;
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
void TCPMessengerDispatcher::createSession(TCPSocket* socket, string peer) {
	TCPSocket* peerSocket = this->sockets[peer];
	if (this->sockets[peer] == NULL) {
		this->deleteSocket(peer);
		TCPMessengerServer::sendCommandToPeer(socket, SESSION_REFUSED);
	} else if (socket->destIpAndPort() == peer) {
		TCPMessengerServer::sendCommandToPeer(socket, SESSION_REFUSED);
	} else {
		TCPSocket* peerSocket = this->sockets[peer];
		this->createBroker(socket, peerSocket);
	}
}

/*
 * Create the multiple TCP socket listener
 */
void TCPMessengerDispatcher::createMultipleTCPSocketListener() {
	delete this->multiSocketListener;
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->multiSocketListener->addSockets(this->getSockets());
}

/*
 * Handle exit socket
 */
void TCPMessengerDispatcher::exit(TCPSocket* socket) {
	cout << "Exit" << endl;
	this->deleteSocket(socket);
	socket->cclose();
	delete socket;
	this->createMultipleTCPSocketListener();

	// todo: delete from maps
}

void TCPMessengerDispatcher::closeBroker(Broker* broker) {
	this->addSocket(broker->firstSocket);
	this->addSocket(broker->secondSocket);
	TCPMessengerServer::sendCommandToPeer(broker->firstSocket,
			CLOSE_SESSION_WITH_PEER);
	TCPMessengerServer::sendCommandToPeer(broker->secondSocket,
			CLOSE_SESSION_WITH_PEER);

	this->brokers.erase(broker);
	delete broker;
	cout << "Close broker" << endl;
	this->createMultipleTCPSocketListener();
}

bool TCPMessengerDispatcher::createRoom(string roomName, TCPSocket* admin) {
	if (this->roomExists(roomName)) {
		return false;
	}

	Room* room = new Room(roomName, this->peersIpToUser[admin->destIpAndPort()],
			admin, this);

	this->rooms.insert(room);
	this->deleteSocket(admin);
	this->createMultipleTCPSocketListener();
	room->start();

	return true;
}

void TCPMessengerDispatcher::closeRoom(Room* room) {
	map<string, TCPSocket*> users = room->users;

	for (std::map<string, TCPSocket*>::const_iterator it = users.begin();
			it != users.end(); it++) {
		this->addSocket(it->second);
	}

	this->rooms.erase(room);
	delete room;

	this->createMultipleTCPSocketListener();
}

void TCPMessengerDispatcher::leaveRoom(TCPSocket* socket) {
	this->addSocket(socket);
	this->createMultipleTCPSocketListener();

	TCPMessengerServer::sendCommandToPeer(socket, SUCCESSFULY_LEFT_ROOM);
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
	this->createMultipleTCPSocketListener();

	TCPMessengerServer::sendCommandToPeer(socket, SUCCESSFULY_JOIN_ROOM);

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

