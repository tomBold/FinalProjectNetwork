#include "AuthDispatcher.h"

AuthDispatcher::AuthDispatcher(TCPMessengerServer* tcpMessengerServer) {
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->start();
	this->tcpMessengerServer = tcpMessengerServer;
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
	this->sockets[socket->fromAddr()] = socket;
	this->multiSocketListener->addSocket(socket);
}

/*
 * Delete socket by socket
 */
void AuthDispatcher::deleteSocket(TCPSocket* socket) {
	std::string destIpAndPort = socket->fromAddr();
	this->deleteSocket(destIpAndPort);
}

/*
 * Delete socket by key
 */
void AuthDispatcher::deleteSocket(string socketKey) {
	this->sockets.erase(this->sockets.find(socketKey));
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
	TCPSocket* sock = this->sockets[socket->fromAddr()];

	if (sock == NULL) {
		this->deleteSocket(sock);

		return false;
	}

	return true;
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
	case (LOGIN): {
		string userNameAndPassword = TCPMessengerServer::readDataFromPeer(
				socket);
		string name;
		string password;

		int index = userNameAndPassword.find(" ");
		name = userNameAndPassword.substr(0, index);
		password = userNameAndPassword.substr(index + 1);

		if (Users::login(name, password)) {
			cout << "Login failed, please check the user name and password"
					<< endl;
			TCPMessengerServer::sendCommandToPeer(socket, LOGIN_FAILED);
		} else {
			this->userLogin(socket, name);
		}

		break;
	}
	case (REGISTER): {
		string userNameAndPassword = TCPMessengerServer::readDataFromPeer(
				socket);
		string name;
		string password;

		int index = userNameAndPassword.find(" ");
		name = userNameAndPassword.substr(0, index);
		password = userNameAndPassword.substr(index + 1);

		if (Users::create(name, password)) {
			cout << name << " register successfully" << endl;
			this->userLogin(socket, name);
		} else {
			cout << name << " register failed" << endl;

			if (Users::contains(name)) {
				cout << "The user: " << name << " already exists" << endl;
			}
		}

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
	delete this->multiSocketListener;
	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->multiSocketListener->addSockets(this->getSockets());
}

/*
 * Handle exit socket
 */
void AuthDispatcher::exit(TCPSocket* socket) {
	cout << "Exit" << endl;
	this->deleteSocket(socket);
	socket->cclose();
	delete socket;
	this->createMultipleTCPSocketListener();
}

void AuthDispatcher::userLogin(TCPSocket* socket, string name) {
	this->deleteSocket(socket);
	this->createMultipleTCPSocketListener();
	this->tcpMessengerServer->userLogin(socket, name);
}
