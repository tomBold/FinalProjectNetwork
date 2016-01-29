/*
 * Room.h
 *
 *  Created on: Jan 29, 2016
 *      Author: user
 */

#ifndef ROOM_H_
#define ROOM_H_

#include <iostream>
#include <strings.h>
#include <vector>
#include "TCPSocket.h"
#include <map>
using namespace std;

class Room {
private:
	string admin;
	string name;
	map<string, TCPSocket*> users;
public:
	Room(string name, string admin, TCPSocket* adminSocket);
	virtual ~Room();
	bool leave(string name);
	bool join(string user,  TCPSocket* userSocket);
	bool close(string user);
	bool exists(string name);
};

#endif /* ROOM_H_ */
