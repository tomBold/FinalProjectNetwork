/*
 * Broker.h
 * This class handles communication peer to peer
 *
 *  Created on: Dec 18, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#ifndef BROKER_H_
#define BROKER_H_

#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerDispatcher.h"
#include "ServerIO.h"

class Broker: public MThread
{
	TCPMessengerDispatcher* dispatcher;
	MultipleTCPSocketsListener* multiSocketListener;
	bool areBothPeersConnected;

	void sendNewDest();
public:
	TCPSocket* firstSocket;
	TCPSocket* secondSocket;

	Broker();
	Broker(TCPSocket* initiativeSocket, TCPSocket* recvSocket, TCPMessengerDispatcher* dispatcher);
	virtual ~Broker();

	/**
	 * Check if the user ip is in the broker
	 */
	bool isUserInBroker(string ip);

	/**
	 * Run the commands between peers
	 */
	void run();
};

#endif /* BROKER_H_ */
