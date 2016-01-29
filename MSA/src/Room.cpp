#include "Room.h"

Room::Room(string name, string admin, TCPSocket* adminSocket,
		TCPMessengerDispatcher* dispatcher) {
	this->name = name;
	this->admin = admin;
	this->users[admin] = adminSocket;

	this->multiSocketListener = new MultipleTCPSocketsListener();
	this->multiSocketListener->addSocket(adminSocket);
	this->dispatcher = dispatcher;
}

Room::~Room() {
	delete this->multiSocketListener;
}

bool Room::leave(string name) {
	if (name == this->admin) {
		return this->close(name);
	}

	if (!exists(name)) {
		return false;
	}

	// Delete the user
	TCPSocket* userLeave = this->users[name];
	this->users.erase(name);

	MultipleTCPSocketsListener* oldListener = this->multiSocketListener;
	MultipleTCPSocketsListener* newListener = new MultipleTCPSocketsListener();

	// Notify that the user leave the room
	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {
		TCPSocket* socket = it->second;
		TCPMessengerServer::sendCommandToPeer(socket, USER_LEAVE_ROOM_RES);
		TCPMessengerServer::sendDataToPeer(socket, name);
		newListener->addSocket(socket);
		this->sendMsgDest(it->first);
	}

	// Init the listener
	this->multiSocketListener = newListener;
	delete oldListener;

	this->dispatcher->leaveRoom(userLeave);

	return true;
}

bool Room::join(string user, TCPSocket* userSocket) {
	if (exists(name)) {
		return false;
	}
	this->users[user] = userSocket;
	this->multiSocketListener->addSocket(userSocket);

	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {
		TCPSocket* socket = it->second;
		TCPMessengerServer::sendCommandToPeer(socket, USER_JOIN_ROOM_RES);
		TCPMessengerServer::sendDataToPeer(socket, user);
		this->sendMsgDest(it->first);
	}

	return true;
}

bool Room::close(string user) {
	if (user != this->admin) {
		return false;
	}

	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {
		TCPSocket* socket = it->second;
		TCPMessengerServer::sendCommandToPeer(socket, CLOSE_ROOM_RES);
	}

	this->dispatcher->closeRoom(this);

	return true;
}

string Room::getOtherUsersIps(string name) {
	string ips = "";
	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {
		if (it->first != name) {
			TCPSocket* socket = it->second;

			if (ips != "") {
				ips += ",";
			}

			ips += it->second->fromAddr();
		}
	}

	return ips;
}

void Room::sendMsgDest(string name) {
	string ips = this->getOtherUsersIps(name);

	TCPMessengerServer::sendCommandToPeer(this->users[name],
	NEW_MESSAGE_DST_RES);
	TCPMessengerServer::sendDataToPeer(this->users[name], ips);
}

bool Room::exists(string name) {
	return this->users.find(name) != this->users.end();
}

/**
 * Run the messages between peers
 */
void Room::run() {
	bool roomIsOpen = true;
	while (roomIsOpen) {

		// Find who from the sockets will send the next command
		TCPSocket* socket = this->multiSocketListener->listenToSocket();
		string name = this->dispatcher->peersIpToUser[socket->destIpAndPort()];
		// If no one sends a command, try again
		if (socket == NULL) {
			continue;
		}

		int command = TCPMessengerServer::readCommandFromPeer(socket);
		switch (command) {
		// when a SIG_TERM happens, the client sends 0
		case (CLOSE_SESSION_WITH_PEER): {
			if (this->leave(name) && name == this->admin) {
				roomIsOpen = false;
			}

			break;
		}
		case (CLOSE_ROOM_REQ): {
			if (this->close(name)) {
				roomIsOpen = false;
			}

			break;
		}
		default: {
			this->dispatcher->handleSocketCommand(socket, command);
		}
		}
	}
}

string Room::getRoomsUsers() {
	string ips = "";
	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {

		TCPSocket* socket = it->second;

		if (ips != "") {
			ips += ",";
		}

		ips += it->first;
	}
	return ips;
}
