/*
 * Room
 * This class represents a chat room
 *
 *  Created on: Jan 29, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "Room.h"

Room::Room(string name, string admin, TCPSocket* adminSocket,
		TCPMessengerDispatcher* dispatcher) {
	this->name = name;
	this->admin = admin;
	this->users[admin] = adminSocket;
	this->roomIsOpen = true;

	this->multiSocketListener = new ExtendedMultipleTCPSocketListener();
	this->multiSocketListener->addSocket(adminSocket);
	this->dispatcher = dispatcher;

	ServerIO::sendCommandToPeer(adminSocket, IN_EMPTY_ROOM);
	ServerIO::sendDataToPeer(adminSocket, this->name);
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

	this->multiSocketListener->removeSocket(userLeave);

	// Notify that the user leave the room
	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {
		TCPSocket* socket = it->second;
		ServerIO::sendCommandToPeer(socket, USER_LEAVE_ROOM_RES);
		ServerIO::sendDataToPeer(socket, name);

		if (this->users.size() == 1) {
			ServerIO::sendCommandToPeer(socket, IN_EMPTY_ROOM);
			ServerIO::sendDataToPeer(socket, this->name);
		} else {
			this->sendMsgDest(it->first);
		}
	}

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
		ServerIO::sendCommandToPeer(socket, USER_JOIN_ROOM_RES);
		ServerIO::sendDataToPeer(socket, user);
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
		ServerIO::sendCommandToPeer(socket, CLOSE_ROOM_RES);
	}

	this->dispatcher->closeRoom(this);

	return true;
}

string Room::getOtherUsersIps(string name) {
	string ips = "";
	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {
		if (it->first != name) {

			if (ips != "") {
				ips += ",";
			}

			ips += this->dispatcher->getUserP2PAddress(it->second);
		}
	}

	return ips;
}

void Room::sendMsgDest(string name) {
	string ips = this->getOtherUsersIps(name);

	ServerIO::sendCommandToPeer(this->users[name],
	NEW_MESSAGE_DST_RES);
	ServerIO::sendDataToPeer(this->users[name], ips);
}

bool Room::exists(string name) {
	return this->users.find(name) != this->users.end();
}

/**
 * Run the messages between peers
 */
void Room::run() {
	this->roomIsOpen = true;
	while (this->roomIsOpen) {
		// Find who from the sockets will send the next command
		TCPSocket* socket = this->multiSocketListener->listenToSocket();
		// If no one sends a command, try again
		if (socket == NULL) {
			continue;
		}

		string name = this->dispatcher->peersIpToUser[socket->destIpAndPort()];
		int command = ServerIO::readCommandFromPeer(socket);
		switch (command) {
		// when a SIG_TERM happens, the client sends 0
		case (CLOSE_SESSION_WITH_PEER): {
			if (this->leave(name) && name == this->admin) {
				this->roomIsOpen = false;
			}

			break;
		}
		case (CLOSE_ROOM_REQ): {
			if (this->close(name)) {
				this->roomIsOpen = false;
			}

			break;
		}
		default: {
			this->dispatcher->handleSocketCommand(socket, command);
		}
		}
	}

	this->dispatcher->deleteRoom(this);
}

/**
 * Get textual representation of the user list
 */
string Room::getRoomsUsers() {
	string names = "";
	for (std::map<string, TCPSocket*>::const_iterator it = this->users.begin();
			it != this->users.end(); it++) {

		if (names != "") {
			names += ",";
		}

		names += it->first;
	}
	return names;
}
