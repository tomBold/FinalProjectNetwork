/*
 * TCPMessengerDispatcher.h
 *
 * Manages login user requests.
 *
 *  Created on: Feb 3, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include <strings.h>
#include <map>
#include "TCPSocket.h"
#include "ExtendedMultipleTCPSocketListener.h"
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerServer.h"
#include "Broker.h"
#include "Room.h"
#include <set>
#include "ServerIO.h"
#include "Dispatcher.h"
#include "Types.h"

#ifndef TCPMESSENGERDISPATCHER_H_
#define TCPMESSENGERDISPATCHER_H_

class Broker;
class Room;

using namespace std;

class TCPMessengerDispatcher : public Dispatcher{
	set<Broker*> brokers;
	set<Room*> rooms;

	map<ip_and_port, user_name> peerIpToUser;
	map<user_name, ip_and_port> userToPeerIp;
	map<ip_and_port, port> peerIpToPort;
public:
	using Dispatcher::deleteSocket;

	TCPMessengerDispatcher();
	virtual ~TCPMessengerDispatcher();

	/*
	 * Add socket to the map and the multiple tcp socket listener.
	 */
	void addSocket(TCPSocket* socket, user_name name, port port);

	/*
	 * Add socket to the map and the multiple tcp socket listener.
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Delete socket by ip and port.
	 */
	void deleteSocket(ip_and_port ipAndPort);

	/*
	 * Print the sockets keys.
	 */
	void printKeys();

	/*
	 * Gets the socket's command and executes it.
	 */
	void handleSocket(TCPSocket* socket);

	/*
	 * Create a broker.
	 */
	void createBroker(TCPSocket* firstSocket, TCPSocket* secondSocket);

	/**
	 * Create session between two users.
	 */
	void createSession(TCPSocket* socket, user_name userName);

	/**
	 * Return if the socket exists.
	 */
	bool isSocketExists(TCPSocket* socket);

	/**
	 * Disconnect a client.
	 */
	void disconnectClient(TCPSocket* socket);

	/**
	 * handle a socket command.
	 */
	void handleSocketCommand(TCPSocket* socket, int command);

	/**
	 * Close broker.
	 */
	void closeBroker(Broker* broker);

	/**
	 * Create a new room.
	 */
	bool createRoom(room_name room, TCPSocket* admin);

	/**
	 * Close a room.
	 */
	void closeRoom(Room* room);

	/**
	 * Leave room.
	 */
	void leaveRoom(TCPSocket* socket);

	/**
	 * Join room.
	 */
	bool joinRoom(room_name roomName, TCPSocket* socket);

	/**
	 * Get the room if the room exists, otherwise return NULL.
	 */
	Room* roomExists(room_name room);

	/**
	 * return if the user is in active room or broker.
	 */
	bool isUserBusy(user_name name, bool forceLeaveUser);

	/**
	 * Force user to leave his active communication (room or broker).
	 */
	void forceLeaveUser(user_name name);

	/**
	 * Gets the rooms names.
	 */
	string getRoomsNames();

	/**
	 * Gets the connected users.
	 */
	string getConnectUsers();

	/**
	 * Gets the brokers.
	 */
	string getAllBrokers();

	/**
	 * Shutdown.
	 */
	void shutdown();

	/**
	 * Get the peer to peer user address.
	 */
	string getUserP2PAddress(TCPSocket* socket);

	/**
	 * Return if the user is connected.
	 */
	bool isUserConnected(user_name name);

	/**
	 * Delete room.
	 */
	void deleteRoom(Room* room);

	/**
	 * Delete broker.
	 */
	void deleteBroker(Broker* broker);

	/**
	 * Gets user by peer ip and port.
	 */
	string getUserByPeerIp(ip_and_port peerIp);

	/**
	 * Gets peer ip and port by user.
	 */
	string getPeerIpByUser(user_name user);

	/**
	 * Gets udp port by peer ip
	 */
	string getPortByPeerIp(ip_and_port peerIp);
};

#endif /* TCPMESSENGERDISPATCHER_H_ */
