/*
 * UDPMessenger.h
 *
 * It's handle the peer to peer communication.
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
#include <vector>
#include "Utilities/Strings.h"

using namespace std;

class UDPMessenger: public MThread {
private:
	bool isRunning;
	map<string, string> ipAndPortToUsers;
public:
	UDPSocket* mainSocket;

	UDPMessenger();
	~UDPMessenger();

	/**
	 * sends the given message
	 */
	void send(string msg);

	/**
	 * close the messenger
	 */
	void close();

	/**
	 * This method runs in a separate thread, it reads the incoming messages
	 * from the socket and prints the content on the terminal.
	 * The thread should exist when the socket is closed
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
