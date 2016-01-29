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
#include "TCPMessengerDispatcher.h"
#include "MultipleTCPSocketsListener.h"
#include <map>
using namespace std;

class Room: public MThread {
private:
	TCPMessengerDispatcher* dispatcher;
	MultipleTCPSocketsListener* multiSocketListener;
public:
	string admin;
	string name;
	map<string, TCPSocket*> users;
	Room(string name, string admin, TCPSocket* adminSocket,
			TCPMessengerDispatcher* dispatcher);
	virtual ~Room();
	bool leave(string name);
	bool join(string user, TCPSocket* userSocket);
	bool close(string user);
	bool exists(string name);
	void run();
	string getOtherUsersIps(string name);
	void sendMsgDest(string name);
	string getRoomsUsers();
};

#endif /* ROOM_H_ */
