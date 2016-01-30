/*
 * TCPMessengerClient.h
 *
 *  Created on: Jan 30, 2016
 *      Author: user
 */

#ifndef TCPMESSENGERCLIENT_H_
#define TCPMESSENGERCLIENT_H_

#include <strings.h>
#include <string.h>
#include "TCPSocket.h"
#include "UDPMessenger.h"
#include "MThread.h"
#include "ServerIO.h"
#include <map>
#include "TCPMessengerProtocol.h"
#include <iostream>
#include <string>

using namespace std;

class TCPMessengerClient: public MThread, public OnRecieveClbk {
	string user;
	map<string, string> ipAndPortToUsers;
	TCPSocket* socket;
	int status;
	string currentRoom;
	bool isRunning;
	UDPMessenger* udpMessenger;

public:
	TCPMessengerClient();
	~TCPMessengerClient();

	void handleMessage(string msg);

	bool connect(string ip);

	bool isConnected();

	bool disconnect();

	bool getAllUsers();

	bool getConnectedUsers();

	bool getAllRooms();

	bool getRoomsUsers(string room);

	bool login(string name, string password);

	bool registerUser(string name, string password);

	string getServerIp();
	string getPeerName();
	string getPeerIpAndPort();
	string getRoomName();
	bool openSession(string user);
	bool joinRoom(string room);
	bool closeRoom(string room);
	bool exitRoom();
	bool isActiveBroker();
	bool isActiveRoom();
	bool closeActiveSession();
	bool send(string msg);
	int getStatus();
	void run();
};

#endif /* TCPMESSENGERCLIENT_H_ */
