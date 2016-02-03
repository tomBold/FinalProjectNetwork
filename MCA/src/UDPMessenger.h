/*
 * UDPMessenger.h
 *
 * Handles the peer to peer communication between two (or more) clients
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef UDPMESSENGER_H_
#define UDPMESSENGER_H_

#include <pthread.h>
#include "UDPSocket.h"
#include "MThread.h"
#include <map>
#include "Types.h"
#include <vector>
#include "Utilities/Strings.h"
#include <string.h>

using namespace std;

class UDPMessenger: public MThread {
private:
	bool isRunning;
	map<ip_and_port, user_name> ipAndPortToUsers;
public:
	UDPSocket* mainSocket;

	UDPMessenger();
	~UDPMessenger();

	/**
	 * Sends the given message to the destinations
	 */
	void send(string msg);

	/**
	 * Close the messenger and stop listening to that port
	 */
	void close();

	/**
	 * Read all the incoming messages and print them in the correct format
	 */
	void run();

	/**
	 * Get the main socket port
	 */
	int getPort();

	/**
	 * Set the messages destination
	 */
	void setTheMsgDestination(string dst);

	/**
	 * Handle a message
	 */
	void handleMsg(string msg, string ipAndPort);
};

#endif
