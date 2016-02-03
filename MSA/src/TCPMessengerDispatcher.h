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

	void clean();
public:
	using Dispatcher::deleteSocket;

	TCPMessengerDispatcher();
	virtual ~TCPMessengerDispatcher();

	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	void addSocket(TCPSocket* socket, user_name name, port port);

	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Delete socket by key
	 */
	void deleteSocket(ip_and_port socketKey);

	/*
	 * Print the sockets keys
	 */
	void printKeys();

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	void handleSocket(TCPSocket* socket);

	/*
	 * Create a broker
	 */
	void createBroker(TCPSocket* firstSocket, TCPSocket* secondSocket);

	/**
	 * Create session
	 */
	void createSession(TCPSocket* socket, user_name peer);
	bool isSocketExists(TCPSocket* socket);
	void disconnectClient(TCPSocket* socket);
	void handleSocketCommand(TCPSocket* socket, int command);
	void closeBroker(Broker* broker);
	bool createRoom(room_name room, TCPSocket* admin);
	void closeRoom(Room* room);
	void leaveRoom(TCPSocket* socket);
	bool joinRoom(room_name roomName, TCPSocket* socket);
	Room* roomExists(room_name room);
	bool isUserBusy(user_name name);
	void forceLeaveUser(user_name name);
	string getRoomsNames();
	string getConnectUsers();
	string getAllBrokers();
	void shutdown();
	string getUserP2PAddress(TCPSocket* socket);
	bool isUserConnected(user_name name);
	void deleteRoom(Room* room);
	void deleteBroker(Broker* broker);
	string getUserByPeerIp(ip_and_port peerIp);
	string getPeerIpByUser(user_name user);
	string getPortByPeerIp(ip_and_port peerIp);
};

#endif /* TCPMESSENGERDISPATCHER_H_ */
