#include "AuthDispatcher.h"

AuthDispatcher::AuthDispatcher(TCPMessengerServer* tcpMessengerServer) {
	this->tcpMessengerServer = tcpMessengerServer;
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->start();
}

AuthDispatcher::~AuthDispatcher() {
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
void AuthDispatcher::addSocket(TCPSocket* socket) {
	this->sockets[socket->destIpAndPort()] = socket;
	this->multiSocketListener->addSocket(socket);
}

/*
 * Delete socket by socket
 */
void AuthDispatcher::deleteSocket(TCPSocket* socket) {
	this->deleteSocket(socket->destIpAndPort());
}

/*
 * Delete socket by ip and port
 */
void AuthDispatcher::deleteSocket(string ipAndPort) {
	this->sockets.erase(this->sockets.find(ipAndPort));
	this->createMultipleTCPSocketListener();
}

/**
 * Retrieve messages from peers
 */
void AuthDispatcher::run() {
	while (true) {
		TCPSocket* currSocket = this->multiSocketListener->listenToSocket();

		if (currSocket != NULL) {
			this->handleSocket(currSocket);
		}
	}
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

	int command = TCPMessengerServer::readCommandFromPeer(socket);

	switch (command) {
	case (LOGIN_REQ): {
		string name;
		string password;

		this->getUserAndPasswordFromSocket(socket, &name, &password);

		if (Users::login(name, password)) {
			cout << "Login failed, please check the user name and password"
					<< endl;
			TCPMessengerServer::sendCommandToPeer(socket, LOGIN_FAILED);
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
				TCPMessengerServer::sendCommandToPeer(socket, USER_ALREADY_EXISTS_RES);
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
 * Get sockets vector
 */
vector<TCPSocket*> AuthDispatcher::getSockets() {
	vector<TCPSocket*> socketsVec;

	for (std::map<string, TCPSocket*>::const_iterator it =
			this->sockets.begin(); it != this->sockets.end(); it++) {
		socketsVec.push_back(it->second);
	}

	return socketsVec;
}

/*
 * Create the multiple TCP socket listener
 */
void AuthDispatcher::createMultipleTCPSocketListener() {
	MultipleTCPSocketsListener* oldMultipleTCPSocketsListener =
			this->multiSocketListener;
	MultipleTCPSocketsListener* newMultipleTCPSocketsListener =
			new MultipleTCPSocketsListener();
	newMultipleTCPSocketsListener->addSockets(this->getSockets());
	this->multiSocketListener = newMultipleTCPSocketsListener;
	delete oldMultipleTCPSocketsListener;
}

/*
 * Handle exit socket
 */
void AuthDispatcher::disconnectClient(TCPSocket* socket) {
	this->deleteSocket(socket);
	socket->cclose();
	delete socket;
}

void AuthDispatcher::userLogin(TCPSocket* socket, string name) {
	this->deleteSocket(socket);
	this->tcpMessengerServer->userLogin(socket, name);
}

void AuthDispatcher::getUserAndPasswordFromSocket(TCPSocket* socket,
		string* name, string* password) {
	string userNameAndPassword = TCPMessengerServer::readDataFromPeer(socket);

	int index = userNameAndPassword.find(" ");

	(*name) = userNameAndPassword.substr(0, index);
	(*password) = userNameAndPassword.substr(index + 1);
}
