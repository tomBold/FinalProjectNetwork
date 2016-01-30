/*
 * Room.h
 * This class represents a chat room
 *
 *  Created on: Jan 29, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef ROOM_H_
#define ROOM_H_

#include <iostream>
#include <strings.h>
#include <vector>
#include "TCPSocket.h"
#include "TCPMessengerDispatcher.h"
#include "ExtendedMultipleTCPSocketListener.h"
#include <map>
using namespace std;

class Room: public MThread {
private:
	TCPMessengerDispatcher* dispatcher;
	ExtendedMultipleTCPSocketListener* multiSocketListener;
public:
	string admin;
	string name;
	map<string, TCPSocket*> users;

	Room(string name, string admin, TCPSocket* adminSocket,
			TCPMessengerDispatcher* dispatcher);
	virtual ~Room();

	/**
	 * Client leaves room
	 */
	bool leave(string name);

	/**
	 * Client joins room
	 */
	bool join(string user, TCPSocket* userSocket);

	/**
	 * Client tries to close room
	 */
	bool close(string user);

	/**
	 * Check if user is participant this room
	 */
	bool exists(string name);

	/**
	 * Get commands from users
	 */
	void run();

	/**
	 * Get other users ips
	 */
	string getOtherUsersIps(string name);

	/**
	 * Send the new destination to the rooms' users
	 */
	void sendMsgDest(string name);

	/**
	 * Get textual representation of the user list
	 */
	string getRoomsUsers();
};

#endif /* ROOM_H_ */
