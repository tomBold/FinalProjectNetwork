/*
 * Broker.h
 *
 * This class handles the commands of the peer to peer communications
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
#include "Types.h"

class Broker: public MThread
{
	TCPMessengerDispatcher* dispatcher;
	MultipleTCPSocketsListener* multiSocketListener;

	void sendNewDest();
public:
	bool areBothPeersConnected;
	TCPSocket* firstSocket;
	TCPSocket* secondSocket;

	Broker();
	Broker(TCPSocket* initiativeSocket, TCPSocket* recvSocket, TCPMessengerDispatcher* dispatcher);
	virtual ~Broker();

	/**
	 * Check if the user ip is in the broker
	 */
	bool isUserInBroker(ip_and_port ip);

	/**
	 * Run the commands between peers
	 */
	void run();
};

#endif /* BROKER_H_ */
