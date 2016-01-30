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

#ifndef TCPMESSENGERDISPATCHER_H_
#define TCPMESSENGERDISPATCHER_H_

class Broker;
class Room;

class TCPMessengerDispatcher : public MThread{
	map<string, TCPSocket*> sockets;
	ExtendedMultipleTCPSocketListener* multiSocketListener;
	set<Broker*> brokers;
	set<Room*> rooms;
	bool isRunning;

	void clean();
public:
	map<string, string> peersIpToUser;
	map<string, string> userToPeersIp;
	map<string, string> peersIpToPort;

	TCPMessengerDispatcher();
	virtual ~TCPMessengerDispatcher();

	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	void addSocket(TCPSocket* socket, string name, string port);

	/*
	 * Add socket to the map and the multiple tcp socket listener
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Delete socket by socket
	 */
	void deleteSocket(TCPSocket* socket);

	/*
	 * Delete socket by key
	 */
	void deleteSocket(string socketKey);

	/*
	 * Print the sockets keys
	 */
	void printKeys();

	/**
	 * Retrieve messages from peers
	 */
	void run();

	/*
	 * Handle socket - gets the socket's command and executes it
	 */
	void handleSocket(TCPSocket* socket);

	/*
	 * Create a broker
	 */
	void createBroker(TCPSocket* firstSocket, TCPSocket* secondSocket);

	/**
	 * Gets sockets
	 */
	vector<TCPSocket*> getSockets();

	/**
	 * Create session
	 */
	void createSession(TCPSocket* socket, string peer);
	bool isSocketExists(TCPSocket* socket);
	void disconnectClient(TCPSocket* socket);
	void handleSocketCommand(TCPSocket* socket, int command);
	void closeBroker(Broker* broker);
	bool createRoom(string room, TCPSocket* admin);
	void closeRoom(Room* room);
	void leaveRoom(TCPSocket* socket);
	bool joinRoom(string roomName, TCPSocket* socket);
	Room* roomExists(string room);
	bool isUserBusy(string name);
	void forceLeaveUser(string name);
	string getRoomsNames();
	string getConnectUsers();
	string getAllBrokers();
	void shutdown();
	string getUserP2PAddress(TCPSocket* socket);
	bool isUserConnected(string name);
};

#endif /* TCPMESSENGERDISPATCHER_H_ */
