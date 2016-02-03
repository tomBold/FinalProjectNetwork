/*
 * TCPMessengerClient.h
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
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

class TCPMessengerClient: public MThread {
	string user;

	TCPSocket* socket;
	int status;
	string currentConversation;
	bool isRunning;
	UDPMessenger* udpMessenger;

	bool isLoggedIn();
	bool sendCommand(int command);
public:
	TCPMessengerClient();
	~TCPMessengerClient();

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
	string getConversation();
	bool openSession(string user);
	bool joinRoom(string room);
	bool closeRoom(string room);
	bool exitRoom();
	bool isActiveBroker();
	bool isActiveRoom();
	bool closeActiveSession();
	bool send(string msg);
	void printStatus();
	void run();
	void setStatus(int status);
};

#endif /* TCPMESSENGERCLIENT_H_ */
