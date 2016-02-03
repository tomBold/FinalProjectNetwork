#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

/***********************   TCPMessengerServer implementation ******************************/

TCPMessengerServer::TCPMessengerServer() {
	this->serverSocket = new TCPSocket(MSNGR_PORT);
	this->dispatcher = new TCPMessengerDispatcher();
	this->authDispatcher = new AuthDispatcher(this);
	this->start();
}

/*
 * Close the server
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
 * Listen and add sockets to the dispatcher
 */
void TCPMessengerServer::run() {
	while (this->serverSocket != NULL) {
		TCPSocket* listener = this->serverSocket->listenAndAccept();
		this->authDispatcher->addSocket(listener);
	}
}

void TCPMessengerServer::userLogin(TCPSocket* peer, string name) {
	ServerIO::sendCommandToPeer(peer, PORT_INIT_REQ);

	int command = ServerIO::readCommandFromPeer(peer);

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

bool TCPMessengerServer::isUserConnected(string name) {
	return this->dispatcher->userToPeersIp.find(name)
			!= this->dispatcher->userToPeersIp.end();
}

string TCPMessengerServer::getConnectUsers() {
	string users = this->dispatcher->getConnectUsers();

	if (users.empty()) {
		return "There is no connected users.";
	}

	return "Connected users: " + users;
}

string TCPMessengerServer::getAllBrokers() {
	string brokers = this->dispatcher->getAllBrokers();

	if (brokers.empty()) {
		return "There is no sessions.";
	}

	return "Sessions: " + brokers;
}

string TCPMessengerServer::getRoomsNames() {
	string rooms = this->dispatcher->getRoomsNames();

	if (rooms.empty()) {
		return "There is no rooms";
	}

	return "Rooms: " + rooms;
}

string TCPMessengerServer::getRoomsUsers(string roomName){
	Room* room = dispatcher->roomExists(roomName);

	if (room) {
		return "Room " + roomName + " users: " + room->getRoomsUsers();
	}

	return "Room " + roomName + " does not exist.";
}
