#include "Room.h"

Room::Room(string name, string admin, TCPSocket* adminSocket) {
	this->name = name;
	this->admin = admin;
	this->users[admin] = adminSocket;
}

Room::~Room() {
	// TODO Auto-generated destructor stub
}

bool Room::leave(string name) {
	if (!exists(name)) {
		return false;
	}

	// TODO:
}

bool Room::join(string user, TCPSocket* userSocket) {
	if (exists(name)) {
		return false;
	}

	this->users[user] = userSocket;

	// TODO:

	return true;
}

bool Room::close(string user) {
	if (user != this->admin) {
		return false;
	}

	// TODO: delete and disconnect all
	return true;
}
bool Room::exists(string name) {
	return this->users.find(name) != this->users.end();
}
