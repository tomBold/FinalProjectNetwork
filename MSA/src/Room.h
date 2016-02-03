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
#include "ServerIO.h"
#include <map>
#include "Types.h"

using namespace std;

class Room: public MThread {
private:
	TCPMessengerDispatcher* dispatcher;
	ExtendedMultipleTCPSocketListener* multiSocketListener;
public:
	user_name admin;
	room_name name;
	map<user_name, TCPSocket*> users;
	bool roomIsOpen;

	Room(room_name name, user_name admin, TCPSocket* adminSocket,
			TCPMessengerDispatcher* dispatcher);
	virtual ~Room();

	/**
	 * Client leaves room
	 */
	bool leave(user_name name);

	/**
	 * Client joins room
	 */
	bool join(user_name user, TCPSocket* userSocket);

	/**
	 * Client tries to close room
	 */
	bool close(user_name user);

	/**
	 * Check if user is participant this room
	 */
	bool exists(user_name name);

	/**
	 * Get commands from users
	 */
	void run();

	/**
	 * Get other users ips
	 */
	string getOtherUsersIps(user_name name);

	/**
	 * Send the new destination to the rooms' users
	 */
	void sendMsgDest(user_name name);

	/**
	 * Get textual representation of the user list
	 */
	string getRoomsUsers();
};

#endif /* ROOM_H_ */
